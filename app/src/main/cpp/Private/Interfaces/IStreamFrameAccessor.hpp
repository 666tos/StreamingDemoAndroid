//
//  IStreamFrameAccessor.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef IStreamFrameAccessor_hpp
#define IStreamFrameAccessor_hpp

#include "IStreamStateAccessor.hpp"
#include <cstdint>
#include <unistd.h>

namespace StreamingEngine {
    class IStreamFrameAccessor: public IStreamStateAccessor {
    public:
        virtual bool hasFramesCacheCapacity() const = 0;
        virtual int64_t targetFrameIndex() const = 0;
    };
}

#endif /* IStreamFrameAccessor_hpp */
