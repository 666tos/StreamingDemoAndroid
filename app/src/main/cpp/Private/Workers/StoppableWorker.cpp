//
//  StoppableWorker.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/20/18.
//

#include "StoppableWorker.hpp"
#include "MutexMacro.hpp"

#include <chrono>

using namespace std;
using namespace StreamingEngine;

StoppableWorker::StoppableWorker() {
}

StoppableWorker::~StoppableWorker() {
    stop();
}

void StoppableWorker::start() {
    if (thread_) {
        return;
    }
    
    thread_ = new thread(StoppableWorker::threadProc, this);
}

// Request the thread to stop by setting value in promise object
void StoppableWorker::stop() {
    {
        synchronize_scope(mutex_);
        isThreadCancelled_ = true;
    }
    
    if (thread_ && (thread_->joinable())) {
        thread_->join();
    }
    
    delete thread_;
    thread_ = nullptr;
}

#pragma mark -
#pragma mark Private

void StoppableWorker::doRun() {
    // Check if thread is requested to stop ?
    while (true) {
        synchronize_scope(mutex_);
        if (isThreadCancelled_) {
            return;
        }
        
        run();
    }
}

void StoppableWorker::threadProc(StoppableWorker *worker) {
    worker->doRun();
}
