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
#include "Playlist.hpp"
#include "RawData.hpp"
#include "Timestamp.hpp"

extern "C" {
#include "StreamState.h"
}

namespace StreamingEngine {
    class ITSPartLoaderService;
    class IDecryptorKeyLoaderService;
    class IStreamStateDelegate;
    class StreamImpl;
    
    class Stream {
    private:
        StreamImpl *impl_;
        
    public:
        Stream(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoader, IDecryptorKeyLoaderService *decryptorKeyLoader, Playlist *playlist);
        ~Stream();
        
        /**
         * Returns state of stream
         */
        StreamState state();
        
        /**
         * Sets downloaded @rawData for @part.
         * Once data is set, it can be decoded when necessary.
         */
        void setData(RawDataRef rawData, int64_t part);
        
        /**
         * Sets downloaded decryption key @rawData for @url.
         * Once data is set, part using it can be decrypted when necessary.
         */
        void setDecryptionKeyData(RawDataRef rawData, const std::string& url);
        
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
