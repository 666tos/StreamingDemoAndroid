//
//  FrameFactory.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#ifndef FrameFactory_hpp
#define FrameFactory_hpp

#import "Frame.hpp"
#import "Timestamp.hpp"

#import <vector>

class AVFrame;

namespace StreamingEngine {
    /**
     * This is implementation of factory of Flyweight pattern.
     * Main idea of this class is to provide storage for frames and reuse them,
     * avoiding unnecessary allocations/deallocations of buffer's memory inside frames
     */
    
    class FrameFactory {
    private:
        mutable std::mutex usedFramesMutex_;
        mutable std::mutex freeFramesMutex_;
        std::vector<FrameRef> usedFrames_;
        std::vector<FrameRef> freeFrames_;
        
        /**
         * Uses frame adding it to @usedFrames_
         */
        void useFrame(FrameRef frame);
        
        /**
         * Reuses frame removing it from @usedFrames_ and adding to @freeFrames_
         */
        void reuseFrame(FrameRef frame);

        /**
         * Looks for a free frame and returns it, removing it if from @freeFrames,
         * nullptr if no free frames available.
         */
        FrameRef popFreeFrame();
        
        /**
         * Looks for a frame with given @timestamp, discarding older frames.
         * See implementation for details.
         */
        FrameRef performFindFrame(const Timestamp& timestamp);
        
    public:
        FrameFactory();
        ~FrameFactory();
        
        /**
         * Creates frame reusing existing from @freeFrames_ (if it's not empty)
         * or creating new frame.
         * Does not use @config_->framebufferSize_, so total numbers of frames may exceed that value.
         */
        
        void createFrame(AVFrame *avframe, const Timestamp& timestamp);
        
        /**
         * Calls performFindFrame, protecting call with mutex lock/unlock
         */
        FrameRef findFrame(const Timestamp& timestamp);

        /**
         * Moves all @usedFrames_ to @freeFrames_.
         */
        void clearUsedFrames();
        
        /**
         * Number of used frames.
         */
        bool hasFramesCacheCapacity() const;
    };
}

#endif /* FrameFactory_hpp */
