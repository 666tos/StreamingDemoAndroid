//
//  StreamImpl.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "StreamImpl.hpp"
#include "TSPartWorker.hpp"
#include "DecodeWorker.hpp"
#include "Config.hpp"
#include "FrameFactory.hpp"
#include "Log.hpp"
#include "Decryptor.hpp"

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

StreamImpl::StreamImpl(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoaderService, IDecryptorKeyLoaderService *decryptorKeyLoaderService, Playlist *playlist):
    targetTimestamp_(0, 0),
    stateDelegate_(stateDelegate),
    tsPartLoaderService_(tsPartLoaderService),
    decryptorKeyLoaderService_(decryptorKeyLoaderService),
    playlist_(playlist) {

    Util::Log::setLogLevel(Config::defaultConfig().logLevel_);

    initializeFFMpeg();
        
    frameFactory_ = new FrameFactory();
    decodeWorker_ = new Decode::Worker(this, this);
        
    decryptorKeyStorage_ = new DecryptorKeyStorage();
    decryptor_    = new Decryptor(decryptorKeyStorage_, decryptorKeyLoaderService_);
    tsPartWorker_ = new TSPartWorker(decodeWorker_, tsPartLoaderService_, this, decryptor_);
        
    Util::Log(Util::Log::Severity::Info) << "[CONSTRUCTED] playlist: " << playlist->numberOfParts() << " parts";
}

StreamImpl::~StreamImpl() {
    tsPartWorker_->stop();
    decodeWorker_->stop();
    
    delete playlist_;
    delete frameFactory_;

    delete decryptorKeyStorage_;
    delete decryptor_;
    
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

void StreamImpl::setData(RawDataRef rawData, int64_t part) {
    auto item = playlist_->findItem(part);
    
    if (item == nullptr) {
        Util::Log(Util::Log::Severity::Error) << "[Set Data] part not found: " << part;
        return;
    }
    
    Util::Log(Util::Log::Severity::Info) << "LOADED TSPART: " << part;
    
    tsPartWorker_->setData(rawData, item);
}

void StreamImpl::setDecryptionKeyData(RawDataRef rawData, const std::string& url) {
    decryptorKeyStorage_->store(rawData, url);
}

FrameRef StreamImpl::getFrame(double timestamp) {
    Timestamp videoTimestamp(timestamp);
    
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

Playlist::ItemRef StreamImpl::getPart() {
    return playlist_->item(currentTSPartIndex_);
}

Playlist::ItemRef StreamImpl::nextPart(Playlist::ItemRef part) {
    return playlist_->findNext(part);
}

#pragma mark -
#pragma mark Private

void StreamImpl::determineCurrentTSPart(const Timestamp& timestamp) {
    auto currentPart = getPart();
    
    if (currentPart && currentPart->timeRange().contains(timestamp)) {
        // Frame with frameIndex is contained in current part, don't switch parts
        targetTimestamp_ = timestamp;
        return;
    }
    
    size_t itemIndex = playlist_->itemIndexForTimestamp(timestamp);
    
    if (itemIndex != -1) {
        targetTimestamp_ = timestamp;
        setCurrentTSPartIndex(itemIndex);
    }
}

void StreamImpl::setCurrentTSPartIndex(size_t tsPartIndex) {
    if (currentTSPartIndex_ == tsPartIndex) {
        return;
    }
    
    Util::Log(Util::Log::Severity::Info) << "SWITCH TSPART: " << tsPartIndex;
    currentTSPartIndex_ = tsPartIndex;
}

FrameRef StreamImpl::findFrame(const Timestamp& timestamp) {
    return frameFactory_->findFrame(timestamp);
}
