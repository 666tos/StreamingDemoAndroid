//
// Created by M Ivaniushchenko on 1/4/19.
//

#include "JNIStream.h"
#include "JNIStreamStateDelegateImpl.h"
#include "JNITSPartLoaderServiceImpl.h"
#include "Stream.hpp"
#include "IStreamStateDelegate.hpp"
#include "Log.hpp"
#include "RawData.hpp"
#include "JNICore.h"
#include "JNIStreamPool.h"

#include <jni.h>
#include <sys/types.h>
#include <vector>
#include <GLES2/gl2.h>

using namespace std;
using namespace Core;
using namespace StreamingEngine;

JNIEXPORT jlong JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type, jobject tsPartList, jint tsPartListSize, jobject stateDelegate, jobject loader) {
    auto stateDelegateImpl = new JNIStreamStateDelegateImpl(stateDelegate);
    auto loadServiceImpl = new JNITSPartLoaderServiceImpl(loader);

    std::vector<TSPartRef> tsParts;

    jclass listClass = env->GetObjectClass(tsPartList);
    jmethodID getMethodID = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
    jobject tsPart;
    for (int i = 0; i < tsPartListSize; i++) {
        tsPart = env->CallObjectMethod(tsPartList, getMethodID, i);
        jclass tsPartClass = env->GetObjectClass(tsPart);
        jmethodID getDurationMethodID = env->GetMethodID(tsPartClass, "getDuration", "()D");
        jmethodID getIndexMethodID = env->GetMethodID(tsPartClass, "getIndex", "()I");
        jmethodID getUrlMethodID = env->GetMethodID(tsPartClass, "getUrl", "()Ljava/lang/String;");
        const char *str = env->GetStringUTFChars(static_cast<jstring>(env->CallObjectMethod(tsPart, getUrlMethodID)), NULL);
        string url(str);
        //env->R

        Util::Log(Util::Log::Severity::Debugger) << url;

        tsParts.push_back(make_shared<TSPart>(
                env->CallIntMethod(tsPart, getIndexMethodID),
                url,
                env->CallDoubleMethod(tsPart, getDurationMethodID)));
    }

    auto stream = new Stream(stateDelegateImpl, loadServiceImpl, tsParts);
    auto handle = getStreamPool().add(stream);
    stream->start();

    return handle;
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_deleteStream(JNIEnv *env, jclass type, jlong handle) {
    auto stream = getStreamPool().get(handle);
    stream->stop();
    getStreamPool().remove(handle);
}

JNIEXPORT jboolean JNICALL Java_com_example_tos_jni_JNIStream_bindFrame(JNIEnv *env, jclass type, jlong handle, double timestamp, jint textureIDY, jint textureIDU, jint textureIDV, jint uTextureAspectRatio) {
    auto stream = getStreamPool().get(handle);
    if (stream == nullptr) {
        return false;
    }

    auto frame = stream->getFrame(timestamp);
    if (frame == nullptr) {
        return false;
    }

    int width = frame->getWidth();
    int height = frame->getHeight();

    auto yPlane = frame->getPlane(Frame::Plane::Y);
    auto uPlane = frame->getPlane(Frame::Plane::U);
    auto vPlane = frame->getPlane(Frame::Plane::V);

    if ((width == 0) || (height == 0) ||
        (yPlane == nullptr) || (uPlane == nullptr) || (vPlane == nullptr)) {
        return false;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, yPlane->getData());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDU);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width/2, height/2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, uPlane->getData());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureIDV);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width/2, height/2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, vPlane->getData());

    float textureAspectRatio = (float) width / height;
    glUniform1f(uTextureAspectRatio, textureAspectRatio);

    return true;
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_setData(JNIEnv *env, jclass type, jlong handle, jbyteArray data, jint part) {
    auto stream = getStreamPool().get(handle);
    if (stream == nullptr) {
        return;
    }

    int length = env->GetArrayLength(data);

    void *dataBuffer = env->GetPrimitiveArrayCritical(data, NULL);
    RawData *rawData = new RawData(dataBuffer, length);

    env->ReleasePrimitiveArrayCritical(data, dataBuffer, 0);

    stream->setData(rawData, part);

    if (env->ExceptionCheck()) return;
}
