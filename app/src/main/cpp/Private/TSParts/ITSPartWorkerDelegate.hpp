//
//  ITSPartWorkerDelegate.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef ITSPartWorkerDelegate_hpp
#define ITSPartWorkerDelegate_hpp

#include "TSPart.hpp"

namespace StreamingEngine {
    class ITSPartWorkerDelegate {
    public:
        virtual TSPartRef getPart() = 0;
        virtual TSPartRef nextPart(TSPartRef part) = 0;
    };
}

#endif /* ITSPartWorkerDelegate_hpp */
