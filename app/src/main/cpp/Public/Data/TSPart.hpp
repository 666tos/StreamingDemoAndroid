//
//  TSPart.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef TSPart_hpp
#define TSPart_hpp

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
        uint32_t fps_ = 0;
        
        int64_t numberOfFrames_ = 0;
        int64_t frameOffset_ = 0;
        
    public:
        TSPart(int64_t tag, std::string url, int64_t numberOfFrames, uint32_t fps);
        ~TSPart();
        
        void markAsLoading();
        void markAsLoaded();
        void setFrameOffset(int64_t frameOffset);
        
        // Getters
        TSPartState state();
        std::string url();
        int64_t tag();
        int64_t numberOfFrames();
        int64_t frameOffset();
        int64_t maxFrame();
    };
    
    using TSPartRef = std::shared_ptr<TSPart>;
}

#endif /* TSPart_hpp */
