//
//  StreamImpl.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "StreamImpl.hpp"
#include "TSPartLoader.hpp"
#include "TSPartWorker.hpp"
#include "Worker.hpp"
#include "Config.hpp"
#include "FrameFactory.hpp"
#include "Log.hpp"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/avutil.h"
#include "libavutil/log.h"
}

using namespace std;
using namespace StreamingEngine;

StreamImpl::StreamImpl(Config *config, IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoaderService, const std::vector<TSPartRef> &tsParts):
    config_(config),
    stateDelegate_(stateDelegate),
    tsPartLoaderService_(tsPartLoaderService),
    tsParts_(tsParts) {
        
    initializeFFMpeg();
        
    frameFactory_ = new FrameFactory(config_);
    tsPartLoader_ = new TSPartLoader(tsPartLoaderService_);
    tsPartWorker_ = new TSPartWorker(tsPartLoader_, this, config_->advanceDownloadStep_);
    decodeWorker_ = new Decode::Worker(this, this);
    
    Util::Log::setLogLevel(config_->logLevel_);
}

StreamImpl::~StreamImpl() {
    tsPartWorker_->stop();
    decodeWorker_->stop();

    delete config_;
    delete tsPartLoader_;
    delete frameFactory_;
    delete tsPartWorker_;
    delete decodeWorker_;
}

void StreamImpl::initializeFFMpeg() {
    static bool sInitialized = false;
    
    if (!sInitialized) {
        sInitialized = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        av_register_all();
#pragma GCC diagnostic pop
    }
    
    av_log_set_level(config_->ffmpegLogLevel());
}

void StreamImpl::setState(StreamState state) {
    if (state_ == state) {
        return;
    }
    
    state_ = state;
    stateDelegate_->streamStateChanged(state);
}

void StreamImpl::setData(RawData *rawData, int64_t part) {
    TSPartRef tsPart = tsParts_[part];
    tsPart->markAsLoaded();
    
    Decode::WorkerTask *task = new Decode::WorkerTask(part, rawData);
    decodeWorker_->addTask(task);
}

int64_t StreamImpl::targetBitrate() {
    return config_->targetBitrate_;
}

FrameRef StreamImpl::getFrame(int64_t index) {
    switch (state_) {
        case StreamStateReady:
        case StreamStateBuffering: {
            if (index < targetFrameIndex_) {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Flush";
                frameFactory_->clearUsedFrames();
            }
            
            auto frame = findFrame(index);
            
            if (frame) {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Frame found: " << frame->getIndex();
                setState(StreamStateReady);
            }
            else {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Frame NOT found: " << index;
                setState(StreamStateBuffering);
            }
            
            determineCurrentTSPart(index);
            return frame;
        }
            
        default:
            return nullptr;
    }
}

void StreamImpl::start() {
    setState(StreamStateBuffering);
}

void StreamImpl::stop() {
    setState(StreamStateStopped);
}

#pragma mark -
#pragma mark StreamStateProvider
StreamState StreamImpl::state() {
    return state_;
}

#pragma mark -
#pragma mark StreamFrameAccessor

bool StreamImpl::hasFramesCacheCapacity() const {
    return frameFactory_->hasFramesCacheCapacity();
}

int64_t StreamImpl::targetFrameIndex() const {
    return targetFrameIndex_;
}

#pragma mark -
#pragma mark WorkerDelegate

void StreamImpl::addFrame(AVFrame *avframe, int64_t index) {
    if (index < targetFrameIndex_) {
        Util::Log(Util::Log::Severity::Verbose) << "[Add Frame] skipping: " << index;
    }
    else {
        Util::Log(Util::Log::Severity::Verbose) << "[Add Frame] added: " << index;
        frameFactory_->createFrame(avframe, index);
    }
}

#pragma mark -
#pragma mark TSPartWorkerDelegate

TSPartRef StreamImpl::getPart() {
    if ((currentTSPartIndex_ >= 0) && (currentTSPartIndex_ < tsParts_.size())) {
        return tsParts_.at((size_t)currentTSPartIndex_);
    }
    
    return nullptr;
}

TSPartRef StreamImpl::nextPart(TSPartRef tsPart) {
    return findNextPart(tsPart);
}

#pragma mark -
#pragma mark Private

void StreamImpl::determineCurrentTSPart(int64_t frameIndex) {
    TSPartRef currentTSPart = getPart();
    if (currentTSPart &&
        frameIndex >= currentTSPart->frameOffset() &&
        frameIndex < currentTSPart->maxFrame()) {
        targetFrameIndex_ = frameIndex;
        return;
    }
    
    for (auto it = tsParts_.begin(); it != tsParts_.end(); ++it) {
        TSPartRef tsPart = *it;
        if (frameIndex < tsPart->maxFrame()) {
            targetFrameIndex_ = frameIndex;
            setCurrentTSPartIndex(distance(tsParts_.begin(), it));
            break;
        }
        else {
            // ??
        }
    }
}

void StreamImpl::setCurrentTSPartIndex(int64_t tsPartIndex) {
    if (currentTSPartIndex_ == tsPartIndex) {
        return;
    }
    
    Util::Log(Util::Log::Severity::Info) << "SWITCH TSPART: " << tsPartIndex;
    currentTSPartIndex_ = tsPartIndex;
}

TSPartRef StreamImpl::findNextPart(TSPartRef tsPart) {
    auto it = find(tsParts_.begin(), tsParts_.end(), tsPart);
    if (it != tsParts_.end()) {
        auto nextIt = std::next(it, 1);
        if (nextIt != tsParts_.end()) {
            return *nextIt;
        }
    }
    
    return nullptr;
}

FrameRef StreamImpl::findFrame(int64_t index) {
    return frameFactory_->findFrame(index);
}
