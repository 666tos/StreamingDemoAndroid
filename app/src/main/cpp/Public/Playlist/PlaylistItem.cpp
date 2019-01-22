//
//  TSPart.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "PlaylistItem.hpp"

#include <algorithm>

using namespace std;
using namespace StreamingEngine;

#pragma mark -
#pragma mark Public

Playlist::Item::Item(XKeyRef xKey, int64_t tag, std::string url, double duration):
    xKey_(xKey),
    tag_(tag),
    url_(url),
    timeRange_(Timestamp::zero, duration) {
    
}

Playlist::Item::~Item() {
    
}

void Playlist::Item::markAsLoading() {
    state_ = Playlist::Item::State::Loading;
}

void Playlist::Item::markAsLoaded() {
    state_ = Playlist::Item::State::Loaded;
}

void Playlist::Item::markAsDecrypted() {
    state_ = Playlist::Item::State::Decrypted;
}

void Playlist::Item::setStartTime(const Timestamp& startTime) {
    timeRange_.setStart(startTime);
}

#pragma mark -
#pragma mark Getters

const Playlist::XKeyRef& Playlist::Item::xKey() const {
    return xKey_;
}

Playlist::Item::State Playlist::Item::state() const {
    return state_;
}

const std::string &Playlist::Item::url() const {
    return url_;
}

int64_t Playlist::Item::tag() const {
    return tag_;
}

const TimeRange& Playlist::Item::timeRange() const {
    return timeRange_;
}
