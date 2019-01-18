//
//  TSPartWorker.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef TSPartWorker_hpp
#define TSPartWorker_hpp

#include "StoppableWorker.hpp"
#include "DecodeWorker.hpp"
#include "RawData.hpp"
#include "WorkerTask.hpp"

#include <stdio.h>
#include <thread>
#include <map>

namespace StreamingEngine {
    class ITSPartLoaderService;
    class ITSPartWorkerDelegate;
    class PlaylistItem;
    class Decryptor;
    
    class TSPartWorker: public StoppableWorker {
    private:
        const int PROCESSING_INTERVAL = 100; // 0.1s
        
        mutable std::mutex mutex_;
        std::map<int64_t, WorkerTaskRef> decryptTasks_;
        
        Decode::Worker *decodeWorker_;
        ITSPartLoaderService *tsPartLoader_;
        ITSPartWorkerDelegate *delegate_;
        Decryptor *decryptor_;
        const int64_t advanceDownloadStep_;
        
        void findAndLoadPart();
        
        void addDecryptTask(WorkerTaskRef decryptTask);
        WorkerTaskRef getDecryptTask(int64_t tag);
        void removeDecryptTask(int64_t tag);
        
        void performDecryption(WorkerTaskRef decryptTask, Playlist::ItemRef item);
    public:
        TSPartWorker(Decode::Worker *decodeWorker, ITSPartLoaderService *tsPartLoader, ITSPartWorkerDelegate *delegate, Decryptor *decryptor, int64_t advanceDownloadStep = Config::defaultConfig().advanceDownloadStep_);
        ~TSPartWorker();
        
        void setData(RawDataRef rawData, Playlist::ItemRef item);
        
        virtual void run();
    };
}
#endif /* TSPartWorker_hpp */
