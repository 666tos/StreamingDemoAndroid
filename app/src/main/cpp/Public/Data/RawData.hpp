//
//  RawData.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef RawData_hpp
#define RawData_hpp

#include <cstdint>
#include <memory>
#include "Config.hpp"

namespace StreamingEngine {
    class RawData {
    public:
        enum class Allocation {
            Default = 0,
#ifdef __IOS_HARDWARE_DECODING__
            VirtualMemory
#endif
        };

        RawData(const void *data, size_t size, Allocation allocation = Allocation::Default);
        RawData(const RawData &rawData2);
        ~RawData();
        
        uint8_t *getData();
        size_t getSize();
        size_t getAllocatedSize();
        
    private:
        uint8_t *data_ = nullptr;
        size_t allocatedSize_ = 0;
        size_t size_ = 0;
        Allocation allocation_;
        
        void allocateMemory();
        void deallocateMemory();
    };
    
    using RawDataRef = std::shared_ptr<RawData>;
}

#endif /* RawData_hpp */
