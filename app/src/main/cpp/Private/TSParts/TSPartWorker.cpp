//
//  TSPartWorker.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "TSPartWorker.hpp"
#include "TSPart.hpp"
#include "TSPartLoader.hpp"
#include "ITSPartWorkerDelegate.hpp"
#include "Log.hpp"

#include <unistd.h>

using namespace std;
using namespace StreamingEngine;

TSPartWorker::TSPartWorker(TSPartLoader *tsPartLoader, ITSPartWorkerDelegate *delegate, int64_t advanceDownloadStep):
    tsPartLoader_(tsPartLoader),
    delegate_(delegate),
    advanceDownloadStep_(advanceDownloadStep) {
        
    start();
}

TSPartWorker::~TSPartWorker() {
}

void TSPartWorker::findAndLoadPart() {
    TSPartRef part = delegate_->getPart();
    
    for (int i = 0; i <= advanceDownloadStep_; i++) {
        if (!part) {
            return;
        }
        
        switch (part->state()) {
            case TSPartState::Idle:
                Util::Log(Util::Log::Severity::Info) << "LOADING TSPART: " << part->tag();
                tsPartLoader_->load(part);
                return;
                
            case TSPartState::Loading:
                return;
                
            default:
                break;
        }
        
        part = delegate_->nextPart(part);
    }
}
    
void TSPartWorker::run() {
    findAndLoadPart();
    this_thread::sleep_for(chrono::milliseconds(PROCESSING_INTERVAL));
}
