//
// Created by M Ivaniushchenko on 1/7/19.
//

#include "JNILoaderServicesImpl.h"
#include "JNICore.h"
#include "Log.hpp"

using namespace Core;
using namespace StreamingEngine;

JNILoaderServicesImpl::JNILoaderServicesImpl(jobject loader) {
    auto jenv = JNICore::getJNIEnv();
    loader_ = jenv->NewGlobalRef(loader);
}

JNILoaderServicesImpl::~JNILoaderServicesImpl() {
    auto jenv = JNICore::getJNIEnv();
    jenv->DeleteGlobalRef(loader_);
}

// IDecryptorKeyLoaderService:
void JNILoaderServicesImpl::performLoad(const std::string &url) {
    auto jenv = JNICore::getJNIEnv();

    jclass loaderClass = jenv->GetObjectClass(loader_);
    jmethodID jMethodID = jenv->GetMethodID(loaderClass, "loadKey", "(Ljava/lang/String;)V");

    jstring jUrl = jenv->NewStringUTF(url.c_str());
    jenv->CallVoidMethod(loader_, jMethodID, jUrl);

    Util::Log(Util::Log::Severity::Verbose) << "Load key: URL: " << url;
}

void JNILoaderServicesImpl::load(const std::string &url, int64_t tag) {
    auto jenv = JNICore::getJNIEnv();

    jclass loaderClass = jenv->GetObjectClass(loader_);
    jmethodID jMethodID = jenv->GetMethodID(loaderClass, "load", "(Ljava/lang/String;I)V");

    jstring jUrl = jenv->NewStringUTF(url.c_str());
    jenv->CallVoidMethod(loader_, jMethodID, jUrl, tag);

    Util::Log(Util::Log::Severity::Verbose) << "Load: " << tag << " URL: " << url;
}
