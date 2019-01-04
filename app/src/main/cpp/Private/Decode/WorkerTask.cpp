//
//  WorkerTask.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "WorkerTask.hpp"
#include "RawData.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

using namespace std;
using namespace StreamingEngine;
using namespace StreamingEngine::Decode;

WorkerTask::WorkerTask(int64_t index, RawData *rawData):
    index_(index),
    rawData_(rawData) {
    
}

WorkerTask::~WorkerTask() {

}

int64_t WorkerTask::index() {
    return index_;
}

RawDataRef WorkerTask::rawData() {
    return rawData_;
}


