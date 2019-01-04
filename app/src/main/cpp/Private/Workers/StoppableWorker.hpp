//
//  StoppableWorker.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/20/18.
//

#ifndef StoppableWorker_hpp
#define StoppableWorker_hpp

#include <stdio.h>
#include <thread>

namespace StreamingEngine {
    class StoppableWorker {
    private:
        bool isThreadCancelled_ = false;
        std::thread *thread_ = nullptr;
        std::mutex mutex_;
        
        void doRun();
        static void threadProc(StoppableWorker *worker);
                
    public:
        StoppableWorker();
        virtual ~StoppableWorker();
        
        // Task need to provide defination  for this function
        // It will be called by thread function
        virtual void run() = 0;
        
        void start();

        // Request the thread to stop by setting value in promise object
        void stop();
    };
}
#endif /* StoppableWorker_hpp */
