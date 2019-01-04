//
//  Stream.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "Stream.hpp"
#include "StreamImpl.hpp"

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

Stream::Stream(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoader, const std::vector<TSPartRef> &tsParts, Config *config) {
    impl_ = new StreamImpl(config, stateDelegate, tsPartLoader, tsParts);
}

Stream::~Stream() {
    delete impl_;
}

StreamState Stream::state() {
    return impl_->state();
}

void Stream::setData(RawData *rawData, int64_t part) {
    impl_->setData(rawData, part);
}

int64_t Stream::targetBitrate() {
    return impl_->targetBitrate();
}

FrameRef Stream::getFrame(int64_t index) {
    return impl_->getFrame(index);
}

void Stream::start() {
    impl_->start();
}

void Stream::stop() {
    impl_->stop();
}
