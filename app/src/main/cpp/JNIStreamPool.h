//
// Created by M Ivaniushchenko on 1/18/19.
//

#ifndef STREAMINGDEMOANDROID_JNISTREAMPOOL_H
#define STREAMINGDEMOANDROID_JNISTREAMPOOL_H

#include "JNIObjectPool.h"
#include "Stream.hpp"

namespace Core {
    namespace {
        JNIObjectPool<StreamingEngine::Stream> streamPool_;
    }

    JNIObjectPool<StreamingEngine::Stream> &getStreamPool() {
        return streamPool_;
    }
}

#endif //STREAMINGDEMOANDROID_JNISTREAMPOOL_H
