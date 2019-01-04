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

#include <stdio.h>
#include <thread>

namespace StreamingEngine {
    class TSPartLoader;
    class ITSPartWorkerDelegate;
    class TSPart;
    
    class TSPartWorker: public StoppableWorker {
    private:
        const int PROCESSING_INTERVAL = 100; // 0.1s
        
        TSPartLoader *tsPartLoader_;
        ITSPartWorkerDelegate *delegate_;
        const int64_t advanceDownloadStep_;
        
        void findAndLoadPart();
    public:
        TSPartWorker(TSPartLoader *tsPartLoader, ITSPartWorkerDelegate *delegate, int64_t advanceDownloadStep);
        ~TSPartWorker();
        
        virtual void run();
    };
}
#endif /* TSPartWorker_hpp */
