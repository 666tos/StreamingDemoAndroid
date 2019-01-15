//
//  IWorkerDelegate.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef IWorkerDelegate_hpp
#define IWorkerDelegate_hpp

#include "Timestamp.hpp"

struct AVCodec;
struct AVFrame;

namespace StreamingEngine {
    namespace Decode {
        class IWorkerDelegate {
        public:
            virtual void addFrame(AVFrame *frame, const Timestamp &timestamp) = 0;
        };
    }
}

#endif /* IWorkerDelegate_hpp */
