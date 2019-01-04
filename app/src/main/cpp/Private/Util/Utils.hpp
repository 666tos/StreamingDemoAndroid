//
//  Utils.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>

#include "Config.hpp"

namespace StreamingEngine {
    class Utils {
    #ifdef __IOS_HARDWARE_DECODING__
    private:
        static size_t ceilToMultiple(size_t value, size_t multiple);
        static size_t ceilToPageSize(size_t value);
    public:
        static size_t allocateVirtualMemory(void **data, size_t size);
        static void deallocateVirtualMemory(void *data, size_t size);
    #endif
    };
}

#endif /* Utils_hpp */
