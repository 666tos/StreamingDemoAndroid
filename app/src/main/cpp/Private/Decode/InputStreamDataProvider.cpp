//
//  InputStreamDataProvider.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "InputStreamDataProvider.hpp"

#include <algorithm>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavutil/avutil.h"
#include "libavutil/log.h"
}

using namespace std;
using namespace StreamingEngine;

#pragma mark -
#pragma mark Public

InputStreamDataProvider::InputStreamDataProvider(IInputStreamDataProviderDelegate *delegate) :
    delegate_(delegate) {
        
    buffer_ = new uint8_t[bufferSize_];
    ioContext_ = avio_alloc_context(buffer_, bufferSize_, 0, this, &InputStreamDataProvider::read, 0, &InputStreamDataProvider::seek);
}

InputStreamDataProvider::~InputStreamDataProvider() {
//    delete [] buffer_;
//    avio_context_free(&ioContext_);
}

void InputStreamDataProvider::setData(RawDataRef rawData) {
    rawData_ = rawData;
    offset_ = 0;
}

AVIOContext* InputStreamDataProvider::ioContext() {
    return ioContext_;
}

#pragma mark -
#pragma mark Private

int InputStreamDataProvider::read(uint8_t *buffer, int size) {
    if (!rawData_) {
        return 0;
    }
    
    int64_t copy = (rawData_->getSize() - offset_);
    copy = min(copy, (int64_t)size);
    
    if (copy <= 0) {
        delegate_->getNextData();
        return 0;
    }
    else {
        memcpy(buffer, rawData_->getData() + offset_, copy);
        offset_ += copy;
    }
    
    return (int)copy;
}

int64_t InputStreamDataProvider::seek(int64_t pos, int whence) {
    switch (whence) {
        case SEEK_SET:
            offset_ = pos;
            return offset_;
            
        default:
            return -1;
    }
}

#pragma mark -
#pragma mark Static

int InputStreamDataProvider::read(void *opaque, uint8_t *buffer, int size) {
    InputStreamDataProvider *that = reinterpret_cast<InputStreamDataProvider*>(opaque);
    return that->read(buffer, size);
}

int64_t InputStreamDataProvider::seek(void *opaque, int64_t pos, int whence) {
    InputStreamDataProvider *that = reinterpret_cast<InputStreamDataProvider*>(opaque);
    return that->seek(pos, whence);
}

