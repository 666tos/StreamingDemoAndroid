//
//  Worker.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "DecodeWorker.hpp"
#include "Frame.hpp"
#include "FrameReader.hpp"
#include "Log.hpp"
#include "MutexMacro.hpp"

#include <unistd.h>

extern "C" {
#include "libavformat/avformat.h"
}

using namespace std;
using namespace StreamingEngine;
using namespace StreamingEngine::Decode;

Worker::Worker(IStreamFrameAccessor *streamFrameAccessor, IWorkerDelegate *delegate):
    streamFrameAccessor_(streamFrameAccessor),
    delegate_(delegate) {

    frameReader_ = new FrameReader(streamFrameAccessor, delegate);
    decodeInfo_ = new Info();
        
    streamDataProvider_ = new InputStreamDataProvider(this);
    streamDataProvider_->delegate_ = this;
        
    start();
}

Worker::~Worker() {
    avformat_close_input(&decodeInfo_->formatContext_);
    
    delete frameReader_;
    delete decodeInfo_;
    delete streamDataProvider_;
}

void Worker::addTask(WorkerTaskRef task) {
    synchronize_scope(mutex_);
    tasks_.push_back(task);
    
    Util::Log(Util::Log::Severity::Verbose) << "[Add TS task]: " << task->index();
}

WorkerTaskRef Worker::getTask() const {
    synchronize_scope(mutex_);
    if (!streamFrameAccessor_->hasFramesCacheCapacity() || tasks_.empty()) {
        return nullptr;
    }
    
    auto it = tasks_.begin();
    return *it;
}

void Worker::run() {
    uint32_t delay = 10;
    
    switch (streamFrameAccessor_->state()) {
        case StreamStateError:
        case StreamStateUnknown:
        case StreamStateStopped:
            break;
            
        case StreamStateBuffering:
        case StreamStateReady: {
            WorkerTaskRef task = getTask();

            if (task == nullptr) {
                break;
            }
            
            if (task != lastTask_) {
                lastTask_ = task;
                
                Util::Log(Util::Log::Severity::Verbose) << "Switch task: " << task->index();
                streamDataProvider_->setData(task->rawData());
            }
            
            Util::Log(Util::Log::Severity::Debugger) << "Found task: " << task->index();
            
            if (!decodeInfo_->inputFormat_) {
                decodeInfo_->determineFormat(task->rawData(), streamDataProvider_->ioContext());
            }
            
            if (decodeInfo_->inputFormat_) {
                while (streamFrameAccessor_->hasFramesCacheCapacity()) {
                    if (AVFrame *avFrame = frameReader_->readFrame(decodeInfo_)) {
                        Timestamp timestamp = decodeInfo_->calculateTimestamp(avFrame->pts);
                        delegate_->addFrame(avFrame, timestamp);
                    }
                    else {
                        break;
                    }
                }
            }
            
            Util::Log(Util::Log::Severity::Debugger) << "Read all frames";
        }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

#pragma mark -
#pragma mark IInputStreamDataProviderDelegate

void Worker::getNextData() {
    synchronize_scope(mutex_);
    performGetNextData();
}

void Worker::performGetNextData() {
    if (lastTask_ != nullptr) {
        Util::Log(Util::Log::Severity::Verbose) << "[Stream finished reading]: " << lastTask_->index();
        
        auto it = find(tasks_.begin(), tasks_.end(), lastTask_);
        if (it != tasks_.end()) {
            tasks_.erase(it);
        }
        
        streamDataProvider_->setData(nullptr);
    }
    else {
        Util::Log(Util::Log::Severity::Verbose) << "[Stream finished reading]: no last task";
    }
    
    lastTask_ = nullptr;
}
