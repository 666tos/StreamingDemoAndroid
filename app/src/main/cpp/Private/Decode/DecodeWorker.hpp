//
//  Worker.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef Worker_hpp
#define Worker_hpp

#include <stdio.h>
#include <thread>
#include <vector>
#include <memory>

#include "StoppableWorker.hpp"
#include "InputStreamDataProvider.hpp"
#include "IStreamFrameAccessor.hpp"
#include "IWorkerDelegate.hpp"
#include "WorkerTask.hpp"
#include "Info.hpp"
#include "PlaylistItem.hpp"

struct AVFormatContext;
struct AVCodecContext;
struct AVInputFormat;
struct AVStream;
struct AVPacket;
struct AVFrame;

namespace StreamingEngine {
    namespace Decode {
        class Info;
        class FrameReader;
        
        class Worker: public StoppableWorker, private IInputStreamDataProviderDelegate {
        private:
            FrameReader *frameReader_;
            Info *decodeInfo_;
            InputStreamDataProvider *streamDataProvider_;
            mutable std::mutex mutex_;
            std::vector<WorkerTaskRef> tasks_;
            WorkerTaskRef lastTask_;
            
            IStreamFrameAccessor *streamFrameAccessor_;
            IWorkerDelegate *delegate_;
            
            WorkerTaskRef getTask() const;
            
            // IInputStreamDataProviderDelegate
            virtual void getNextData();
            
            void performGetNextData();
            
        public:
            Worker(IStreamFrameAccessor *streamFrameAccessor, IWorkerDelegate *delegate);
            ~Worker();
            
            void addTask(WorkerTaskRef task);
            
            virtual void run();
        };
    }
}
#endif /* TSPartWorker_hpp */
