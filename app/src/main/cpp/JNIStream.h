//
// Created by M Ivaniushchenko on 1/4/19.
//

#ifndef TACX_TRAINING_ANDROID_JNI_STREAM_H
#define TACX_TRAINING_ANDROID_JNI_STREAM_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type, jobject tsPartList, jint tsPartListSize, jobject stateDelegate, jobject loader);
JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_deleteStream(JNIEnv *env, jclass type, jlong handle);

JNIEXPORT jboolean JNICALL Java_com_example_tos_jni_JNIStream_bindFrame(JNIEnv *env, jclass type, jlong handle, jdouble timestamp, jint textureIDY, jint textureIDU, jint textureIDV, jint uTextureAspectRatio);
JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_setData(JNIEnv *env, jclass type, jlong handle, jbyteArray data, jint part);

#ifdef __cplusplus
}
#endif

#endif //TACX_TRAINING_ANDROID_JNI_STREAM_H
