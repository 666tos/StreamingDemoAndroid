//
//  TSPart.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef TSPart_hpp
#define TSPart_hpp

#include "TimeRange.hpp"

#include <cstdint>
#include <string>

namespace StreamingEngine {
    enum class TSPartState {
        Idle = 0,
        Loading,
        Loaded,
    };
    
    class TSPart {
    private:
        TSPartState state_ = TSPartState::Idle;
        std::string url_;
        int64_t tag_ = 0;
        TimeRange timeRange_;
        
    public:
        TSPart(int64_t tag, std::string url, double duration);
        ~TSPart();
        
        void markAsLoading();
        void markAsLoaded();
        void setStartTime(const Timestamp& startTime);
        
        // Getters
        TSPartState state();
        const std::string& url();
        int64_t tag();
        
        const TimeRange& timeRange() const;
    };
    
    using TSPartRef = std::shared_ptr<TSPart>;
}

#endif /* TSPart_hpp */
