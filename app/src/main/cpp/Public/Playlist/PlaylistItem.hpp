//
//  TSPart.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef TSPart_hpp
#define TSPart_hpp

#include "Playlist.hpp"
#include "PlaylistXKey.hpp"
#include "TimeRange.hpp"

#include <cstdint>
#include <string>

namespace StreamingEngine {
    class Playlist::Item {
    public:
        enum class State {
            Idle = 0,
            Loading,
            Loaded,
            Decrypted,
        };
    
        Item(XKeyRef xKey, int64_t tag, std::string url, double duration);
        ~Item();
        
        void markAsLoading();
        void markAsLoaded();
        void markAsDecrypted();
        void setStartTime(const Timestamp& startTime);
        
        // Getters
        const XKeyRef& xKey() const;
        State state() const;
        const std::string& url() const;
        int64_t tag() const;
        const TimeRange& timeRange() const;
        
    private:
        XKeyRef xKey_;
        State state_ = State::Idle;
        std::string url_;
        int64_t tag_ = 0;
        TimeRange timeRange_;
    };
}

#endif /* TSPart_hpp */
