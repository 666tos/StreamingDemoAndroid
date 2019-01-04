//
//  IStreamStateDelegate.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/21/18.
//

#ifndef IStreamStateDelegate_hpp
#define IStreamStateDelegate_hpp

#include "StreamState.h"

namespace StreamingEngine {
    class TSPart;
    
    class IStreamStateDelegate {
    public:
        virtual ~IStreamStateDelegate() {};
        virtual void streamStateChanged(StreamState state) = 0;
    };
}

#endif /* IStreamStateDelegate_hpp */
