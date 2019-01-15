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
#include "TSPart.hpp"
#include "ITSPartWorkerDelegate.hpp"
#include "IStreamStateDelegate.hpp"
#include "ITSPartLoaderService.hpp"

extern "C" {
#include "StreamState.h"
}

namespace StreamingEngine {
    class TSPartLoader;
    class TSPartWorker;
    class Config;
    class FrameFactory;
    
    namespace Decode {
        class Worker;
    }
    
    class StreamImpl: private IStreamFrameAccessor, Decode::IWorkerDelegate, ITSPartWorkerDelegate {
    private:
        Config *config_;
        StreamState state_ = StreamStateStopped;
        
        std::vector<TSPartRef> tsParts_;
        
        FrameFactory *frameFactory_;
        
        int64_t currentTSPartIndex_ = -1;
        
        Timestamp targetTimestamp_;
        
        IStreamStateDelegate *stateDelegate_;
        ITSPartLoaderService *tsPartLoaderService_;
        TSPartLoader *tsPartLoader_;
        TSPartWorker *tsPartWorker_;
        Decode::Worker *decodeWorker_;

        void alignTSParts();
        void initializeFFMpeg();
        void setState(StreamState state);
        
        // StreamFrameAccessor
        virtual bool hasFramesCacheCapacity() const;
        virtual Timestamp targetTimestamp() const;
        
        // WorkerDelegate
        virtual void addFrame(AVFrame *avframe, const Timestamp& timestamp);
        
        // TSPartWorkerDelegate
        virtual TSPartRef getPart();
        virtual TSPartRef nextPart(TSPartRef tsPart);
        
        void determineCurrentTSPart(const Timestamp& timestamp);
        void setCurrentTSPartIndex(int64_t tsPartIndex);
        TSPartRef findNextPart(TSPartRef tsPart);
        FrameRef findFrame(const Timestamp& timestamp);
        
    public:
        // StreamStateProvider
        virtual StreamState state();
        
        StreamImpl(Config *config, IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoaderService, const std::vector<TSPartRef> &tsParts);
        ~StreamImpl();
        void setData(RawData *rawData, int64_t part);
        
        int64_t targetBitrate();
        FrameRef getFrame(double timestamp);
        
        void start();
        void stop();
    };
}

#endif /* StreamImpl_hpp */
