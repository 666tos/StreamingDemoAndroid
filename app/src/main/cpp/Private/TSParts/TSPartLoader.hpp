//
//  TSPartLoader.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef TSPartLoader_hpp
#define TSPartLoader_hpp

#include <cstdint>
#include "TSPart.hpp"

namespace StreamingEngine {
    class ITSPartLoaderService;
    class RawData;
    
    class TSPartLoader {
    private:
        ITSPartLoaderService *tsPartLoaderService_;
        
    public:
        TSPartLoader(ITSPartLoaderService *tsPartLoaderService);
        
        void load(TSPartRef tsPart);
    };
    
}

#endif /* TSPartLoader_hpp */
