//
//  PlaylistXKey.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/8/19.
//

#ifndef PlaylistSessionKey_hpp
#define PlaylistSessionKey_hpp

#include <stdio.h>
#include "Playlist.hpp"
#include "RawData.hpp"

namespace StreamingEngine {
    class Playlist::XKey {
    public:
        XKey(std::string method, std::string uri, RawDataRef ivData, std::string keyFormat, std::string keyFormatVersions);
        ~XKey();
        
        void markAsLoading();
        void markAsLoaded();
        void setFrameOffset(int64_t frameOffset);
        
        // Getters
        const std::string& method();
        const std::string& uri();
        RawDataRef ivData();
        const std::string& keyFormat();
        const std::string& keyFormatVersions();
        
    private:
        std::string method_;
        std::string uri_;
        RawDataRef ivData_;
        std::string keyFormat_;
        std::string keyFormatVersions_;
    };
}
#endif /* PlaylistSessionKey_hpp */
