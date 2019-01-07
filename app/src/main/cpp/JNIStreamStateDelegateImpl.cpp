//
// Created by M Ivaniushchenko on 1/7/19.
//

#include "JNIStreamStateDelegateImpl.h"
#include "JNICore.h"

using namespace Core;

JNIStreamStateDelegateImpl::JNIStreamStateDelegateImpl(jobject delegate) {
    auto env = JNICore::getJNIEnv();
    delegate_ = env->NewGlobalRef(delegate);
}

JNIStreamStateDelegateImpl::~JNIStreamStateDelegateImpl() {
    auto env = JNICore::getJNIEnv();
    env->DeleteGlobalRef(delegate_);
}

void JNIStreamStateDelegateImpl::streamStateChanged(StreamState state) {
    auto env = JNICore::getJNIEnv();

    jclass delegateClass = env->GetObjectClass(delegate_);
    jmethodID methodID = env->GetMethodID(delegateClass, "stateChanged", "(I)V");

    env->CallVoidMethod(delegate_, methodID, state);
}