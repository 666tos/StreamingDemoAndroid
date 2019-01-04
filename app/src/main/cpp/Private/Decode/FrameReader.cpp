//
//  FrameReader.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/18/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "FrameReader.hpp"
#include "IStreamFrameAccessor.hpp"
#include "Info.hpp"

extern "C" {
#include "libavformat/avformat.h"
}

using namespace StreamingEngine;
using namespace StreamingEngine::Decode;

FrameReader::FrameReader(IStreamFrameAccessor *streamFrameAccessor, IWorkerDelegate *delegate):
    streamFrameAccessor_(streamFrameAccessor),
    delegate_(delegate) {
    
}

FrameReader::~FrameReader() {
}

AVFrame *FrameReader::readFrame(Info *decodeInfo) {
    AVPacket packet;
    int error;
    
    bool frameDecoded = false;

    while (1) {
        if ((error = av_read_frame(decodeInfo->formatContext_, &packet))) {
            break;
        }
        
        if (!shouldHandlePacket(&packet, decodeInfo)) {
            continue;
        }
        
        if ((error = avcodec_send_packet(decodeInfo->codecContext_, &packet))) {
            break;
        }
        
        error = avcodec_receive_frame(decodeInfo->codecContext_, decodeInfo->frame_);
        
        if (error == AVERROR(EAGAIN)) {
            continue;
        }
        
        if (error) {
            break;
        }
        
        if (decodeInfo->frame_->pict_type == AV_PICTURE_TYPE_NONE) {
            continue;
        }
        
        frameDecoded = true;
        break;
    }
    
    av_packet_unref(&packet);
    
    if (!frameDecoded) {
        return nullptr;
    }
    
    return decodeInfo->frame_;
}

#pragma mark -
#pragma mark Private

bool FrameReader::shouldHandlePacket(const AVPacket *packet, Info *decodeInfo) {
    if (packet->stream_index != decodeInfo->streamIndex_) {
        return false;
    }
    
    if ((packet->flags & AV_PKT_FLAG_DISCARD) != 0) {
        return false;
    }
    
    return true;
}
