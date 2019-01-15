//
//  Stream.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef Stream_hpp
#define Stream_hpp

#include <unistd.h>
#include <memory>
#include "Frame.hpp"
#include "Config.hpp"
#include "TSPart.hpp"
#include "Timestamp.hpp"

extern "C" {
#include "StreamState.h"
}

namespace StreamingEngine {
    class ITSPartLoaderService;
    class IStreamStateDelegate;
    class StreamImpl;
    class RawData;
    
    class Stream {
    private:
        StreamImpl *impl_;
        
    public:
        Stream(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoader, const std::vector<TSPartRef> &tsParts, Config *config = Config::defaultConfig());
        ~Stream();
        
        /**
         * Returns state of stream
         */
        StreamState state();
        
        /**
         * Sets downloaded @rawData for @part.
         * Once data is set, it can be decoded when necessary.
         */
        void setData(RawData *rawData, int64_t part);
        
        int64_t targetBitrate();
        
        /**
         * Gets frame for requested timestamp.
         * All frames before @timestamp are cleared.
         * If frame not cached, stream state changes to StreamStateBuffering.
         */
        FrameRef getFrame(double timestamp);
        
        /**
         * Sets state to StreamStateBuffering, allowing workers to start decode if possible.
         */
        void start();
        
        /**
         * Sets state to StreamStateStopped, not allowing workers to decode frames.
         */
        void stop();
    };
}

#endif /* Stream_hpp */
