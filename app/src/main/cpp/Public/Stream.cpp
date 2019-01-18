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

Stream::Stream(IStreamStateDelegate *stateDelegate, ITSPartLoaderService *tsPartLoader, IDecryptorKeyLoaderService *decryptorKeyLoader, Playlist *playlist) {
    impl_ = new StreamImpl(stateDelegate, tsPartLoader, decryptorKeyLoader, playlist);
}

Stream::~Stream() {
    delete impl_;
}

StreamState Stream::state() {
    return impl_->state();
}

void Stream::setData(RawDataRef rawData, int64_t part) {
    impl_->setData(rawData, part);
}

void Stream::setDecryptionKeyData(RawDataRef rawData, const std::string& url) {
    impl_->setDecryptionKeyData(rawData, url);
}

FrameRef Stream::getFrame(double timestamp) {
    return impl_->getFrame(timestamp);
}

void Stream::start() {
    impl_->start();
}

void Stream::stop() {
    impl_->stop();
}
