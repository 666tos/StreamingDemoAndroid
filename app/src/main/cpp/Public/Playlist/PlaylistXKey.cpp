//
//  PlaylistXKey.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/8/19.
//

#include "PlaylistXKey.hpp"

using namespace std;
using namespace StreamingEngine;

Playlist::XKey::XKey(string method, string uri, RawDataRef ivData, string keyFormat, string keyFormatVersions):
    method_(method),
    uri_(uri),
    ivData_(ivData),
    keyFormat_(keyFormat),
    keyFormatVersions_(keyFormatVersions) {
    
}

Playlist::XKey::~XKey() {
    
}

const string& Playlist::XKey::method() {
    return method_;
}

const string& Playlist::XKey::uri() {
    return uri_;
}

RawDataRef Playlist::XKey::ivData() {
    return ivData_;
}

const string& Playlist::XKey::keyFormat() {
    return keyFormat_;
}

const string& Playlist::XKey::keyFormatVersions() {
    return keyFormatVersions_;
}
