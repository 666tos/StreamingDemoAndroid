//
//  FrameFactory.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#include "FrameFactory.hpp"
#include "MutexMacro.hpp"
#include <memory>

using namespace std;
using namespace StreamingEngine;

FrameFactory::FrameFactory() {
    
}

FrameFactory::~FrameFactory() {
    
}

void FrameFactory::createFrame(AVFrame *avframe, const Timestamp& timestamp) {
    auto frame = popFreeFrame();
    
    if (frame != nullptr) {
        frame->setFrame(avframe, timestamp);
        useFrame(frame);
    }
    else {
        useFrame(make_shared<Frame>(avframe, timestamp));
    }
}

FrameRef FrameFactory::findFrame(const Timestamp& timestamp) {
    synchronize_scope(usedFramesMutex_);
    return performFindFrame(timestamp);
}

void FrameFactory::clearUsedFrames() {
    synchronize_scope(usedFramesMutex_);
    synchronize_scope(freeFramesMutex_);
    
    copy(usedFrames_.begin(), usedFrames_.end(), back_inserter(freeFrames_));
    usedFrames_.clear();
}

bool FrameFactory::hasFramesCacheCapacity() const {
    synchronize_scope(usedFramesMutex_);
    return usedFrames_.size() < Config::defaultConfig().framebufferSize_;
}

#pragma mark -
#pragma mark Private

void FrameFactory::useFrame(FrameRef frame) {
    synchronize_scope(usedFramesMutex_);
    usedFrames_.push_back(frame);
}

void FrameFactory::reuseFrame(FrameRef frame) {
    synchronize_scope(freeFramesMutex_);
    freeFrames_.push_back(frame);
}

FrameRef FrameFactory::popFreeFrame() {
    synchronize_scope(freeFramesMutex_);
    
    auto it = freeFrames_.begin();
    
    if (it != freeFrames_.end()) {
        auto frame = *it;
        freeFrames_.erase(it);
        
        return frame;
    }
    
    return nullptr;
}

FrameRef FrameFactory::performFindFrame(const Timestamp& timestamp) {
    for (auto it = usedFrames_.begin(); it != usedFrames_.end(); ) {
        auto frame = *it;
        
        if (frame->getTimestamp() < timestamp) {
            /**
             * @frame's timestamp is smaller than @timestamp requested.
             * @frame is not needed anymore and can be deleted.
             */
            
            reuseFrame(frame);
            it = usedFrames_.erase(it);
        }
        else if (frame->getTimestamp() == timestamp) {
            /**
             * Happy case, @frame's timestamp equals to @timestamp requested.
             */
            
            return frame;
        } else if (Timestamp::approximatelyEqual(frame->getTimestamp(), timestamp, Config::defaultConfig().frameTimestampTolerance_)) {
            /**
             * Corner case, @frame's timestamp is greater than @timestamp requested.
             * This means that frame with @timestamp was not decoded, but the next one was.
             * Checking how far is @frame's timestamp from @timestamp - if it's not too far, return @frame.
             */
            
            Util::Log(Util::Log::Severity::Warning) << "[Find Frame] Requested frame: " << timestamp << " but only found frame: " << frame->getTimestamp();
            return frame;
        } else {
            /**
             * Should never happen since frames are always sorted.
             */
            
            break;
        }
    }
    
    return nullptr;
}
