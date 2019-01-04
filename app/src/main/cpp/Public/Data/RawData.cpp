//
//  RawData.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "RawData.hpp"
#include <memory>
#include "Utils.hpp"

using namespace StreamingEngine;

RawData::RawData(const void *data, size_t size, Allocation allocation):
    allocation_(allocation),
    size_(size) {
    
    allocateMemory();
    memcpy(data_, data, size);
}

RawData::~RawData() {
    deallocateMemory();
}

uint8_t *RawData::getData() {
    return data_;
}

size_t RawData::getSize() {
    return size_;
}

size_t RawData::getAllocatedSize() {
    return allocatedSize_;
}

#pragma mark -
#pragma mark Private

void RawData::allocateMemory() {
    switch (allocation_) {
        case Allocation::Default:
            data_ = new uint8_t[size_];
            allocatedSize_ = size_;
            break;
            
#ifdef __IOS_HARDWARE_DECODING__
        case Allocation::VirtualMemory:
            allocatedSize_ = Utils::allocateVirtualMemory((void**)&data_, size_);
            break;
#endif
    }
}

void RawData::deallocateMemory() {
    switch (allocation_) {
        case Allocation::Default:
            delete [] data_;
            break;
            
#ifdef __IOS_HARDWARE_DECODING__
        case Allocation::VirtualMemory:
            Utils::deallocateVirtualMemory(data_, allocatedSize_);
            break;
#endif
    }
}
