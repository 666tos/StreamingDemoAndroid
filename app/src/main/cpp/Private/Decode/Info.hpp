//
//  DecodeInfo.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/18/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef DecodeInfo_hpp
#define DecodeInfo_hpp

#include <memory>
#include "RawData.hpp"
#include "Timestamp.hpp"

extern "C" {
#include "libavutil/rational.h"
#include "libavutil/pixfmt.h"
}

struct AVCodecContext;
struct AVInputFormat;
struct AVFormatContext;
struct AVIOContext;
struct AVCodec;
struct AVFrame;

namespace StreamingEngine {
    class WorkerTaskContext;
    
    namespace Decode {
        class Info {
        private:
            static const int PROBE_INPUT_SIZE = 4096;
            
            void determineInputFormat(RawDataRef rawData);
            bool prepareForReading();
            void createFormatContext(AVIOContext *ioContext);
            
            bool canOpenCodec(AVCodec *codec, AVCodecContext *codecContext);
            
        #ifdef __IOS_HARDWARE_DECODING__
            static AVPixelFormat getFormat(AVCodecContext* context, AVPixelFormat const formats[]);
        #endif
            
        public:
            AVFrame *frame_;
            AVInputFormat *inputFormat_ = nullptr;
            AVCodecContext *codecContext_ = nullptr;
            AVFormatContext *formatContext_ = nullptr;
            
            int streamIndex_ = -1;
            
            int64_t ptsStart_ = 0;
            AVRational timeBase_;
            
            Info();
            ~Info();
            
            void determineFormat(RawDataRef rawData, AVIOContext *ioContext);
            Timestamp calculateTimestamp(int64_t pts);
        };
    }
}

#endif /* DecodeInfo_hpp */
