//
// Created by M Ivaniushchenko on 1/7/19.
//

#ifndef STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H
#define STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H

#include "ITSPartLoaderService.hpp"
#include "IDecryptorKeyLoaderService.hpp"
#include <jni.h>

namespace Core {

class JNILoaderServicesImpl : public StreamingEngine::ITSPartLoaderService, public StreamingEngine::IDecryptorKeyLoaderService {
    public:
        JNILoaderServicesImpl(jobject loader);
        virtual ~JNILoaderServicesImpl();

        // IDecryptorKeyLoaderService:
        virtual void performLoad(const std::string &url);

    protected:
        // ITSPartLoaderService:
        virtual void load(const std::string& url, int64_t tag);

    private:
        jobject loader_;
    };
}

#endif //STREAMINGDEMOANDROID_JNITSPARTLOADERSERVICEIMPL_H
