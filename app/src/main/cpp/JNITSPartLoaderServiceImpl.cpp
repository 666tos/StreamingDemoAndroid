//
// Created by M Ivaniushchenko on 1/7/19.
//

#include "JNITSPartLoaderServiceImpl.h"
#include "JNICore.h"
#include "Log.hpp"

using namespace Core;
using namespace StreamingEngine;

JNITSPartLoaderServiceImpl::JNITSPartLoaderServiceImpl(jobject loader) {
    auto env = JNICore::getJNIEnv();
    loader_ = env->NewGlobalRef(loader);
}

JNITSPartLoaderServiceImpl::~JNITSPartLoaderServiceImpl() {
    auto env = JNICore::getJNIEnv();
    env->DeleteGlobalRef(loader_);
}

void JNITSPartLoaderServiceImpl::load(TSPartRef tsPart) {
    auto env = JNICore::getJNIEnv();

    jclass loaderClass = env->GetObjectClass(loader_);
    jmethodID methodID = env->GetMethodID(loaderClass, "load", "(Ljava/lang/String;I)V");

    jstring url = env->NewStringUTF(tsPart->url().c_str());
    env->CallVoidMethod(loader_, methodID, url, tsPart->tag());

    Util::Log(Util::Log::Severity::Verbose) << "Load: " << tsPart->tag() << " URL: " << tsPart->url();
};