//
//  TSPart.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "TSPart.hpp"

#include <algorithm>

using namespace std;
using namespace StreamingEngine;

#pragma mark -
#pragma mark Public

TSPart::TSPart(int64_t tag, std::string url, int64_t numberOfFrames, uint32_t fps):
    tag_(tag),
    url_(url),
    fps_(fps),
    numberOfFrames_(numberOfFrames),
    frameOffset_(0) {
    
}

TSPart::~TSPart() {
    
}

void TSPart::markAsLoading() {
    state_ = TSPartState::Loading;
}

void TSPart::markAsLoaded() {
    state_ = TSPartState::Loaded;
}

void TSPart::setFrameOffset(int64_t frameOffset) {
    frameOffset_ = frameOffset;
}

#pragma mark -
#pragma mark Getters

TSPartState TSPart::state() {
    return state_;
}

std::string TSPart::url() {
    return url_;
}

int64_t TSPart::tag() {
    return tag_;
}

int64_t TSPart::numberOfFrames() {
    return numberOfFrames_;
}

int64_t TSPart::frameOffset() {
    return frameOffset_;
}

int64_t TSPart::maxFrame() {
    return frameOffset_ + numberOfFrames_;
}
