//
//  Frame.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "Frame.hpp"
#include "Log.hpp"
#include "RawData.hpp"

extern "C" {
#include <libavutil/frame.h>
}

#ifdef __IOS_HARDWARE_DECODING__
#include "CoreVideo/CoreVideo.h"
#endif

using namespace std;
using namespace StreamingEngine;

Frame::Frame(AVFrame* frame, int64_t index) {
    setFrame(frame, index);
}

Frame::~Frame() {
    for (int i = 0; i < static_cast<int>(Plane::Count); i++) {
        delete planes_[i];
    }
}

void Frame::setFrame(AVFrame* frame, int64_t index) {
    index_ = index;
    width_ = frame->width;
    height_= frame->height;
    
#ifdef __IOS_HARDWARE_DECODING__
    copyFrameData420(frame);
#else
    copyFrameData422(frame);
#endif
}

int64_t Frame::getIndex() {
    return index_;
}

int Frame::getWidth() {
    return width_;
}

int Frame::getHeight() {
    return height_;
}

RawData* Frame::getPlane(Plane plane) {
    return planes_[static_cast<int>(plane)];
}

#pragma mark -
#pragma mark Private

void Frame::copyPlaneData(void *sourceData, int plane, size_t dataSize) {
    auto buffer = planes_[plane];
    
    if ((buffer == nullptr) || (buffer->getAllocatedSize() < dataSize)) {
        Util::Log(Util::Log::Severity::Debugger) << "[Frame] Allocated buffer: " << dataSize;
        delete buffer;
        
    #ifdef __IOS_HARDWARE_DECODING__
        buffer = new RawData(sourceData, dataSize, RawData::Allocation::VirtualMemory);
    #else
        buffer = new RawData(sourceData, dataSize, RawData::Allocation::Default);
    #endif
        
        planes_[plane] = buffer;
    }
    else {
        Util::Log(Util::Log::Severity::Debugger) << "[Frame] Copied data: " << dataSize;
        memcpy(buffer->getData(), sourceData, dataSize);
    }
}

#ifdef __IOS_HARDWARE_DECODING__

void Frame::copyFrameData420(AVFrame *frame) {
    CVPixelBufferRef pixelBuffer = (CVPixelBufferRef)frame->data[3];
    CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
    OSType pixelFormat = CVPixelBufferGetPixelFormatType(pixelBuffer);
    
    if (pixelFormat != kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange) {
        CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
        Util::Log(Util::Log::Severity::Error) << "wrong pixel format!";
        
        // Not possible to decode frame, set size to 0 to treat this frame as empty
        width_ = 0;
        height_ = 0;
        return;
    }
    
    for (int plane = 0; plane < 2; plane++) {
        size_t height = CVPixelBufferGetHeightOfPlane(pixelBuffer, plane);
        size_t bytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, plane);
        size_t dataSize = height * bytesPerRow;
        void *planeDataSource = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, plane);
        
        copyPlaneData(planeDataSource, plane, dataSize);
    }
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
}

#else

void Frame::copyFrameData422(AVFrame *frame) {
    copyFrameData(frame, 0, height_);
    copyFrameData(frame, 1, height_/2);
    copyFrameData(frame, 2, height_/2);
}

void Frame::copyFrameData(AVFrame *frame, int plane, int height) {
    int lineSize = frame->linesize[plane];
    size_t dataSize = lineSize * height;
    copyPlaneData(frame->data[plane], plane, dataSize);
}

#endif
