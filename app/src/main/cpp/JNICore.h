//
// Created by M Ivaniushchenko on 1/4/19.
//

#ifndef STREAMINGDEMOANDROID_JNICORE_H
#define STREAMINGDEMOANDROID_JNICORE_H

#include <jni.h>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM * jvm , void *reserved);

#ifdef __cplusplus
}
#endif

namespace Core {

class JNICore {
    public:
        JNICore(JavaVM *vm);

        static void init(JavaVM *vm);
        static void terminate();

        static JNICore *getInstance();
        static JNIEnv *getJNIEnv();

    private:
        static std::unique_ptr<JNICore> instance_;

        JavaVM *vm_;
};

}

#endif //STREAMINGDEMOANDROID_JNICORE_H
