//
// Created by M Ivaniushchenko on 1/4/19.
//

#ifndef TACX_TRAINING_ANDROID_JNI_STREAM_H
#define TACX_TRAINING_ANDROID_JNI_STREAM_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type, jobject loader);
JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_deleteStream(JNIEnv *env, jclass type);

JNIEXPORT jboolean JNICALL Java_com_example_tos_jni_JNIStream_getFrame(JNIEnv *env, jclass type, jlong index);

#ifdef __cplusplus
}
#endif

#endif //TACX_TRAINING_ANDROID_JNI_STREAM_H
