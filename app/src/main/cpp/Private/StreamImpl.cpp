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

#ifdef __ANDROID_HARDWARE_DECODING__
#include "libavcodec/jni.h"
#include "JNICore.h"
#endif
}

using namespace std;
using namespace StreamingEngine;

StreamImpl::StreamImpl(Config *config, IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoaderService, const std::vector<TSPartRef> &tsParts):
    config_(config),
    targetTimestamp_(0, 0),
    stateDelegate_(stateDelegate),
    tsPartLoaderService_(tsPartLoaderService),
    tsParts_(tsParts) {

    Util::Log::setLogLevel(config_->logLevel_);

    alignTSParts();
    initializeFFMpeg();
        
    frameFactory_ = new FrameFactory(config_);
    tsPartLoader_ = new TSPartLoader(tsPartLoaderService_);
    tsPartWorker_ = new TSPartWorker(tsPartLoader_, this, config_->advanceDownloadStep_);
    decodeWorker_ = new Decode::Worker(config_, this, this);
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

void StreamImpl::alignTSParts() {
    // Iterage through TSParts and set frame offset

    double totalOffset = 0;

    for (auto it = tsParts_.begin(); it != tsParts_.end(); ++it) {
        TSPartRef tsPart = *it;

        tsPart->setStartTime(Timestamp(totalOffset, config_->frameTimestampDelta_));
        totalOffset += tsPart->timeRange().duration();
    }
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

#ifdef __ANDROID_HARDWARE_DECODING__
    av_jni_set_java_vm(Core::JNICore::getVM(), NULL);
#endif
}

void StreamImpl::setState(StreamState state) {
    if (state_ == state) {
        return;
    }
    
    state_ = state;
    stateDelegate_->streamStateChanged(state);
}

void StreamImpl::setData(RawData *rawData, int64_t part) {
    auto it = find_if(tsParts_.begin(), tsParts_.end(), [&part](const TSPartRef& obj) {return obj->tag() == part;});
    if (it == tsParts_.end()) {
        Util::Log(Util::Log::Severity::Error) << "[Set Data] part not found: " << part;
        return;
    }

    Util::Log(Util::Log::Severity::Info) << "LOADED TSPART: " << part;
    TSPartRef tsPart = *it;
    tsPart->markAsLoaded();
    
    Decode::WorkerTask *task = new Decode::WorkerTask(part, rawData);
    decodeWorker_->addTask(task);
}

int64_t StreamImpl::targetBitrate() {
    return config_->targetBitrate_;
}

FrameRef StreamImpl::getFrame(double timestamp) {
    Timestamp videoTimestamp(timestamp, config_->frameTimestampDelta_);
    
    switch (state_) {
        case StreamStateReady:
        case StreamStateBuffering: {
            if (videoTimestamp < targetTimestamp_) {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Flush";
                frameFactory_->clearUsedFrames();
            }
            
            auto frame = findFrame(videoTimestamp);
            
            if (frame) {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Frame found: " << frame->getTimestamp();
                setState(StreamStateReady);
            }
            else {
                Util::Log(Util::Log::Severity::Verbose) << "[Get Frame] Frame NOT found: " << videoTimestamp;
                setState(StreamStateBuffering);
            }
            
            determineCurrentTSPart(videoTimestamp);
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

Timestamp StreamImpl::targetTimestamp() const {
    return targetTimestamp_;
}

#pragma mark -
#pragma mark WorkerDelegate

void StreamImpl::addFrame(AVFrame *avframe, const Timestamp &timestamp) {
    if (timestamp < targetTimestamp_) {
        Util::Log(Util::Log::Severity::Verbose) << "[Add Frame] skipping: " << timestamp;
    }
    else {
        Util::Log(Util::Log::Severity::Verbose) << "[Add Frame] added: " << timestamp;
        frameFactory_->createFrame(avframe, timestamp);
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

void StreamImpl::determineCurrentTSPart(const Timestamp& timestamp) {
    TSPartRef currentTSPart = getPart();
    if (currentTSPart && currentTSPart->timeRange().contains(timestamp)) {
        // Frame with frameIndex is contained in current part, don't switch parts
        targetTimestamp_ = timestamp;
        return;
    }
    
    for (auto it = tsParts_.begin(); it != tsParts_.end(); ++it) {
        TSPartRef tsPart = *it;
        if (tsPart->timeRange().contains(timestamp)) {
            targetTimestamp_ = timestamp;
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

FrameRef StreamImpl::findFrame(const Timestamp& timestamp) {
    return frameFactory_->findFrame(timestamp);
}
