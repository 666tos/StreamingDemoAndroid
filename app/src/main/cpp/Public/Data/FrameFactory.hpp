//
//  FrameFactory.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#ifndef FrameFactory_hpp
#define FrameFactory_hpp

#import "Frame.hpp"

#import <vector>

class AVFrame;

namespace StreamingEngine {
    class Config;
    
    /**
     * This is implementation of factory of Flyweight pattern.
     * Main idea of this class is to provide storage for frames and reuse them,
     * avoiding unnecessary allocations/deallocations of buffer's memory inside frames
     */
    
    class FrameFactory {
    private:
        Config *config_;
        
        std::mutex usedFramesMutex_;
        std::mutex freeFramesMutex_;
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
        
    public:
        FrameFactory(Config *config);
        ~FrameFactory();
        
        /**
         * Creates frame reusing existing from @freeFrames_ (if it's not empty)
         * or creating new frame.
         * Does not use @config_->framebufferSize_, so total numbers of frames may exceed that value.
         */
        
        void createFrame(AVFrame *avframe, int64_t index);
        
        /**
         * Looks for a frame with given @index, discarding older frames.
         * See implementation for details.
         */
        FrameRef findFrame(int64_t index);

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
