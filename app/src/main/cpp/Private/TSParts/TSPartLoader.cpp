//
//  TSPartLoader.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "TSPartLoader.hpp"
#include "TSPart.hpp"
#include "ITSPartLoaderService.hpp"
#include "RawData.hpp"

using namespace StreamingEngine;

TSPartLoader::TSPartLoader(ITSPartLoaderService *tsPartLoaderService):
    tsPartLoaderService_(tsPartLoaderService) {
    
}

void TSPartLoader::load(TSPartRef tsPart) {
    tsPart->markAsLoading();
    tsPartLoaderService_->load(tsPart);
}
