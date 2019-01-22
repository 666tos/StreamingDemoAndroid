//
//  DecryptorKeyStorage.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/11/19.
//

#ifndef DecryptorKeyStorage_hpp
#define DecryptorKeyStorage_hpp

#include <stdio.h>
#include <map>

#include "RawData.hpp"

namespace StreamingEngine {
    class DecryptorKeyStorage {
    private:
        mutable std::mutex mutex_;
        std::map<std::string, RawDataRef> decryptionKeys_;
        
    public:
        DecryptorKeyStorage();
        ~DecryptorKeyStorage();
        
        void store(RawDataRef key, const std::string& url);
        RawDataRef key(const std::string& url) const;
    };
}

#endif /* DecryptorKeyStorage_hpp */
