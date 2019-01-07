//
// Created by M Ivaniushchenko on 1/7/19.
//

#ifndef STREAMINGDEMOANDROID_JNISTREAMSTATEDELEGATEIMPL_H
#define STREAMINGDEMOANDROID_JNISTREAMSTATEDELEGATEIMPL_H

#include "IStreamStateDelegate.hpp"
#include <jni.h>

namespace Core {

    class JNIStreamStateDelegateImpl : public StreamingEngine::IStreamStateDelegate {
    public:
        JNIStreamStateDelegateImpl(jobject delegate);
        virtual ~JNIStreamStateDelegateImpl();

        virtual void streamStateChanged(StreamState state);
    private:
        jobject delegate_;
    };
}

#endif //STREAMINGDEMOANDROID_JNISTREAMSTATEDELEGATEIMPL_H
