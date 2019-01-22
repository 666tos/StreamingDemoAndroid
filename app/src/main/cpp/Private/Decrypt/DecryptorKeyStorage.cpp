//
//  DecryptorKeyStorage.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/11/19.
//

#include "DecryptorKeyStorage.hpp"
#include "MutexMacro.hpp"

using namespace std;
using namespace StreamingEngine;

DecryptorKeyStorage::DecryptorKeyStorage() {
}

DecryptorKeyStorage::~DecryptorKeyStorage() {
}

void DecryptorKeyStorage::store(RawDataRef key, const std::string& url) {
    synchronize_scope(mutex_);
    decryptionKeys_[url] = key;
}

RawDataRef DecryptorKeyStorage::key(const std::string& url) const {
    synchronize_scope(mutex_);
    auto it = decryptionKeys_.find(url);
    
    if (it != decryptionKeys_.end()) {
        return it->second;
    }
    
    return nullptr;
}
