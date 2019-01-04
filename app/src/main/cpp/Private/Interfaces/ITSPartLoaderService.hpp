//
//  ITSPartLoaderService.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef ITSPartLoaderService_hpp
#define ITSPartLoaderService_hpp

#include "TSPart.hpp"

namespace StreamingEngine {
    class TSPart;
    
    class ITSPartLoaderService {
    public:
        virtual ~ITSPartLoaderService() {};
        virtual void load(TSPartRef tsPart) = 0;
    };
}

#endif /* ITSPartLoaderService_hpp */
