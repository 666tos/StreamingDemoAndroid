//
//  DecodeInfo.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/18/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "Info.hpp"
#include "RawData.hpp"
#include "Log.hpp"

extern "C" {
#ifdef __IOS_HARDWARE_DECODING__
#include "libavcodec/videotoolbox.h"
#endif
#include "libavformat/avformat.h"
#include "libavutil/frame.h"
#include "libavutil/pixdesc.h"
}

using namespace std;
using namespace StreamingEngine;
using namespace StreamingEngine::Decode;

Info::Info():
 frame_(av_frame_alloc()) {
     
}

Info::~Info() {
    av_frame_free(&frame_);
    avcodec_free_context(&codecContext_);
    avformat_free_context(formatContext_);
}

void Info::determineFormat(RawDataRef rawData, AVIOContext *ioContext) {
    determineInputFormat(rawData);
    
    if (!inputFormat_) {
        return;
    }
    
    createFormatContext(ioContext);
    prepareForReading();
}

int64_t Info::calculatePTS(int64_t pts) {
    double result = (pts != AV_NOPTS_VALUE) ? pts - ptsStart_ : 0;

    // TODO: replace 30 with actual FPS
    result *= 30.0 * av_q2d(timeBase_);
    return result;
}

#pragma mark -
#pragma mark Private

void Info::determineInputFormat(RawDataRef rawData) {
    int totalProbeDataBufferSize = PROBE_INPUT_SIZE + AVPROBE_PADDING_SIZE;
    unsigned char probeDataBuffer[totalProbeDataBufferSize];
    memset(probeDataBuffer, 0, totalProbeDataBufferSize);
    memcpy(probeDataBuffer, rawData->getData(), PROBE_INPUT_SIZE);
    
    AVProbeData probeData;
    probeData.buf = probeDataBuffer;
    probeData.buf_size = PROBE_INPUT_SIZE;
    probeData.filename = "stream";
    probeData.mime_type = "video/MP2T";
    
    inputFormat_ = av_probe_input_format(&probeData, 1);
}

bool Info::prepareForReading() {
    int error;
    
    if ((error = avformat_open_input(&formatContext_, 0, 0, 0))) {
        return false;
    }
    
    if ((error = avformat_find_stream_info(formatContext_, 0))) {
        return false;
    }
    
    AVCodec* videoCodec = nullptr;
    auto videoStreamIndex = av_find_best_stream(formatContext_, AVMEDIA_TYPE_VIDEO, -1, -1, &videoCodec, 0);
    
    if (videoStreamIndex < 0) {
        return false;
    }
    
    AVStream* stream = formatContext_->streams[videoStreamIndex];
    AVCodecContext *codecContext = avcodec_alloc_context3(videoCodec);
    
    if (auto result = avcodec_parameters_to_context(codecContext, stream->codecpar) < 0) {
        avcodec_free_context(&codecContext);
        return false;
    }
    
#ifdef __IOS_HARDWARE_DECODING__
    codecContext->get_format = Info::getFormat;
#endif

    if (!canOpenCodec(videoCodec, codecContext)) {
        avcodec_free_context(&codecContext);
        return false;
    }
    
    // Discard all streams except video
    for (auto i = 0; i < formatContext_->nb_streams; ++i) {
        formatContext_->streams[i]->discard = AVDISCARD_ALL;
    }
    
    stream->discard = AVDISCARD_DEFAULT;
    
    streamIndex_ = videoStreamIndex;
    codecContext_ = codecContext;
    ptsStart_ = stream->start_time;
    timeBase_ = stream->time_base;

    return true;
}

#ifdef __IOS_HARDWARE_DECODING__
AVPixelFormat Info::getFormat(AVCodecContext* context, AVPixelFormat const formats[]) {
    for (auto i = 0; formats[i] != AV_PIX_FMT_NONE; ++i) {
        Util::Log(Util::Log::Severity::Verbose) << "get_format: format[" << i << "]: " << av_get_pix_fmt_name(formats[i]);
        
        if (formats[i] == AV_PIX_FMT_VIDEOTOOLBOX) {
            
            // Force video toolbox hardware acceleration
            auto result = av_videotoolbox_default_init(context);
            
            if (result < 0) {
                Util::Log(Util::Log::Severity::Error) << "av_videotoolbox_default_init failed: " << av_err2str(result);
                return formats[0];
            }
            else {
                Util::Log(Util::Log::Severity::Info) << "get_format: selecting AV_PIX_FMT_VIDEOTOOLBOX";
                return AV_PIX_FMT_VIDEOTOOLBOX;
            }
        }
    }
    return formats[0];
}
#endif

void Info::createFormatContext(AVIOContext *ioContext) {
    AVFormatContext *formatContext = avformat_alloc_context();
    formatContext->pb = ioContext;
//    formatContext->flags |= AVFMT_FLAG_CUSTOM_IO;
    formatContext->iformat = inputFormat_;
    
    formatContext_ = formatContext;
}

bool Info::canOpenCodec(AVCodec *codec, AVCodecContext *codecContext) {
    int error;
    
    if ((error = avcodec_open2(codecContext, codec, 0))) {
        return false;
    }
    
    return true;
}
