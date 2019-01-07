//
// Created by M Ivaniushchenko on 1/4/19.
//

#include "JNICore.h"

using namespace Core;

JNIEXPORT jint
JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    JNICore::init(jvm);
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *jvm, void *reserved) {
    JNICore::terminate();
}

std::unique_ptr<JNICore> JNICore::instance_;

void JNICore::init(JavaVM *vm) {
    instance_.reset(new JNICore(vm));
}

void JNICore::terminate() {
    instance_.reset();
}

JNICore* JNICore::getInstance()  {
    return instance_.get();
}

JNICore::JNICore(JavaVM *vm):
    vm_(vm) {
}

JNIEnv* JNICore::getJNIEnv() {
    JNIEnv* env;
    getInstance()->vm_->AttachCurrentThread(&env, NULL);
    return env;
}

JavaVM* JNICore::getVM() {
    return getInstance()->vm_;
}