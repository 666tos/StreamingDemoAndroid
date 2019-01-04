//
//  IStreamStateAccessor.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef IStreamStateAccessor_hpp
#define IStreamStateAccessor_hpp

extern "C" {
#include "StreamState.h"
}

namespace StreamingEngine {
    class IStreamStateAccessor {
    public:
        virtual StreamState state() = 0;
    };
}

#endif /* IStreamStateAccessor_hpp */
