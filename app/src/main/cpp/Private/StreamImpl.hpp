//
//  StreamImpl.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef StreamImpl_hpp
#define StreamImpl_hpp

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <thread>

#include "InputStreamDataProvider.hpp"
#include "IStreamStateAccessor.hpp"
#include "IStreamFrameAccessor.hpp"

#include "IWorkerDelegate.hpp"
#include "Frame.hpp"
#include "PlaylistItem.hpp"
#include "ITSPartWorkerDelegate.hpp"
#include "IStreamStateDelegate.hpp"
#include "ITSPartLoaderService.hpp"

extern "C" {
#include "StreamState.h"
}

namespace StreamingEngine {
    class TSPartWorker;
    class FrameFactory;
    class Decryptor;
    class IDecryptorKeyLoaderService;
    class DecryptorKeyStorage;
    
    namespace Decode {
        class Worker;
    }
    
    class StreamImpl: private IStreamFrameAccessor, Decode::IWorkerDelegate, ITSPartWorkerDelegate {
    private:
        StreamState state_ = StreamStateStopped;
        
        Playlist *playlist_;
        
        FrameFactory *frameFactory_;
        
        size_t currentTSPartIndex_ = -1;
        
        Timestamp targetTimestamp_;
        
        IStreamStateDelegate *stateDelegate_;
        ITSPartLoaderService *tsPartLoaderService_;
        
        IDecryptorKeyLoaderService *decryptorKeyLoaderService_;
        DecryptorKeyStorage *decryptorKeyStorage_;
        Decryptor *decryptor_;
        
        Decode::Worker *decodeWorker_;
        
        TSPartWorker *tsPartWorker_;

        void initializeFFMpeg();
        void setState(StreamState state);
        
        // StreamFrameAccessor
        virtual bool hasFramesCacheCapacity() const;
        virtual Timestamp targetTimestamp() const;
        
        // WorkerDelegate
        virtual void addFrame(AVFrame *avframe, const Timestamp& timestamp);
        
        // TSPartWorkerDelegate
        virtual Playlist::ItemRef getPart();
        virtual Playlist::ItemRef nextPart(Playlist::ItemRef tsPart);
        
        void determineCurrentTSPart(const Timestamp& timestamp);
        void setCurrentTSPartIndex(size_t tsPartIndex);
        FrameRef findFrame(const Timestamp& timestamp);
        
    public:
        // StreamStateProvider
        virtual StreamState state();
        
        StreamImpl(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoaderService, IDecryptorKeyLoaderService *decryptorKeyLoaderService, Playlist *playlist);
        ~StreamImpl();
        void setData(RawDataRef rawData, int64_t part);
        void setDecryptionKeyData(RawDataRef rawData, const std::string& url);
        
        FrameRef getFrame(double timestamp);
        
        void start();
        void stop();
    };
}

#endif /* StreamImpl_hpp */
