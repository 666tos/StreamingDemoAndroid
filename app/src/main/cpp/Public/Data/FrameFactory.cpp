//
//  FrameFactory.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#include "FrameFactory.hpp"
#include "Utils.hpp"
#include <memory>

using namespace std;
using namespace StreamingEngine;

FrameFactory::FrameFactory(Config *config):
    config_(config) {
    
}

FrameFactory::~FrameFactory() {
    
}

void FrameFactory::createFrame(AVFrame *avframe, const Timestamp& timestamp) {
    freeFramesMutex_.lock();
    
    auto it = freeFrames_.begin();
    FrameRef frame = nullptr;
    
    if (it != freeFrames_.end()) {
        frame = *it;
        frame->setFrame(avframe, timestamp);
        freeFrames_.erase(it);
    }
    else {
        frame = make_shared<Frame>(avframe, timestamp);
    }
    
    freeFramesMutex_.unlock();
    
    useFrame(frame);
}

FrameRef FrameFactory::findFrame(const Timestamp& timestamp) {
    usedFramesMutex_.lock();
    
    FrameRef resultFrame = nullptr;
    
    for (auto it = usedFrames_.begin(); it != usedFrames_.end(); ) {
        auto frame = *it;
        
        if (frame->getTimestamp() < timestamp) {
            /**
             * @frame's timestamp is smaller than @timestamp requested.
             * @frame is not needed anymore and can be deleted.
             */
            
            reuseFrame(*it);
            it = usedFrames_.erase(it);
        }
        else if (frame->getTimestamp() == timestamp) {
            /**
             * Happy case, @frame's timestamp equals to @timestamp requested.
             */
            
            resultFrame = frame;
            break;
        } else if (Timestamp::approximatelyEqual(frame->getTimestamp(), timestamp, config_->frameTimestampDelta_)) {
            /**
             * Corner case, @frame's timestamp is greater than @timestamp requested.
             * This means that frame with @timestamp was not decoded, but the next one was.
             * Checking how far is @frame's timestamp from @timestamp - if it's not too far, return @frame.
             */
            
            Util::Log(Util::Log::Severity::Warning) << "[Find Frame] Requested frame: " << timestamp << " but only found frame: " << frame->getTimestamp();
            resultFrame = frame;
            break;
        } else {
            /**
             * Should never happen since frames are always sorted.
             */
            
            break;
        }
    }
    
    usedFramesMutex_.unlock();
    return resultFrame;
}

void FrameFactory::clearUsedFrames() {
    usedFramesMutex_.lock();
    freeFramesMutex_.lock();
    
    copy(usedFrames_.begin(), usedFrames_.end(), back_inserter(freeFrames_));
    usedFrames_.clear();
    
    freeFramesMutex_.unlock();
    usedFramesMutex_.unlock();
}

bool FrameFactory::hasFramesCacheCapacity() const {
    return usedFrames_.size() < config_->framebufferSize_;
}

#pragma mark -
#pragma mark Private

void FrameFactory::useFrame(FrameRef frame) {
    usedFramesMutex_.lock();
    
    usedFrames_.push_back(frame);
    
    usedFramesMutex_.unlock();
}

void FrameFactory::reuseFrame(FrameRef frame) {
    freeFramesMutex_.lock();
    
    freeFrames_.push_back(frame);
    
    freeFramesMutex_.unlock();
}
