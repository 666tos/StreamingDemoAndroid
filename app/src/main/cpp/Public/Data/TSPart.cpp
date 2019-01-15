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

TSPart::TSPart(int64_t tag, std::string url, double duration):
    tag_(tag),
    url_(url),
    timeRange_(Timestamp::zero, duration) {
    
}

TSPart::~TSPart() {
    
}

void TSPart::markAsLoading() {
    state_ = TSPartState::Loading;
}

void TSPart::markAsLoaded() {
    state_ = TSPartState::Loaded;
}

void TSPart::setStartTime(const Timestamp& startTime) {
    timeRange_.setStart(startTime);
}

#pragma mark -
#pragma mark Getters

TSPartState TSPart::state() {
    return state_;
}

const std::string& TSPart::url() {
    return url_;
}

int64_t TSPart::tag() {
    return tag_;
}

const TimeRange& TSPart::timeRange() const {
    return timeRange_;
}
