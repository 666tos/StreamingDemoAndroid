//
//  Playlist.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/8/19.
//

#include "Playlist.hpp"
#include "PlaylistItem.hpp"
#include "Log.hpp"

using namespace std;
using namespace StreamingEngine;

Playlist::Playlist(const vector<Playlist::ItemRef> &parts):
    parts_(parts) {
        
    alignParts();
}

Playlist::~Playlist() {
    
}

Playlist::ItemRef Playlist::findNext(Playlist::ItemRef item) {
    auto it = find(parts_.begin(), parts_.end(), item);
    if (it != parts_.end()) {
        auto nextIt = std::next(it, 1);
        if (nextIt != parts_.end()) {
            return *nextIt;
        }
    }
    
    return nullptr;
}

Playlist::ItemRef Playlist::findItem(int64_t tag) {
    auto it = find_if(parts_.begin(), parts_.end(), [&tag](const ItemRef& obj) { return obj->tag() == tag; });
    if (it == parts_.end()) {
        return nullptr;
    }
    
    return *it;
}

Playlist::ItemRef Playlist::item(size_t index) {
    if ((index >= 0) && (index < parts_.size())) {
        return parts_.at(index);
    }
    
    return nullptr;
}

size_t Playlist::itemIndexForTimestamp(const Timestamp& timestamp) {
    for (auto it = parts_.begin(); it != parts_.end(); ++it) {
        auto part = *it;
        if (part->timeRange().contains(timestamp)) {
            return distance(parts_.begin(), it);
        }
        else {
            // ??
        }
    }
    
    return -1;
}

size_t Playlist::numberOfParts() {
    return parts_.size();
}

#pragma mark -
#pragma mark Private

void Playlist::alignParts() {
    // Iterage through TSParts and set frame offset
    double totalOffset = 0;
    
    for (auto it = parts_.begin(); it != parts_.end(); ++it) {
        auto part = *it;
        
        part->setStartTime(Timestamp(totalOffset));
        totalOffset += part->timeRange().duration();
    }
}
