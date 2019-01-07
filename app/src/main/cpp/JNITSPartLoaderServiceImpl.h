//
// Created by M Ivaniushchenko on 1/7/19.
//

#ifndef STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H
#define STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H

#include "ITSPartLoaderService.hpp"
#include <jni.h>

namespace Core {

class JNITSPartLoaderServiceImpl : public StreamingEngine::ITSPartLoaderService {
    public:
        JNITSPartLoaderServiceImpl(jobject loader);
        virtual ~JNITSPartLoaderServiceImpl();

        virtual void load(StreamingEngine::TSPartRef tsPart);

    private:
        jobject loader_;
    };
}

#endif //STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H
