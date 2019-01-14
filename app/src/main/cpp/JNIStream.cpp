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

#include <jni.h>
#include <sys/types.h>
#include <vector>
#include <GLES2/gl2.h>

using namespace std;
using namespace Core;
using namespace StreamingEngine;

static Stream *sStream_ = nullptr;

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type, jobject tsPartList, jint tsPartListSize, jobject stateDelegate, jobject loader) {
    auto stateDelegateImpl = new JNIStreamStateDelegateImpl(stateDelegate);
    auto loadServiceImpl = new JNITSPartLoaderServiceImpl(loader);

    std::vector<TSPartRef> tsParts;

    //char charBuffer[1024];

    /**
     * For this playlist qualities are 127000, 380000, 720000, 1150000, 2210000 and 3520000
     * Video chunks urls are 0, 60000000, 120000000, etc
     */

    /*const char *quality = "2210000";
    const int chunkStep = 6;
    const char *formatString = "https://nikitasplendo1-euwe.streaming.media.azure.net/125d908d-0b7f-42df-b8c2-cfb956ff37e8/T205523.ism/QualityLevels(%s)/Fragments(video=%i0000000,format=m3u8-aapl-v3,audiotrack=aac_und_2_127_2_1)";*/

    jclass listClass = env->GetObjectClass(tsPartList);
    jmethodID getMethodID = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
    jobject tsPart;
    for (int i = 0; i < tsPartListSize; i++) {
        tsPart = env->CallObjectMethod(tsPartList, getMethodID, i);
        jclass tsPartClass = env->GetObjectClass(tsPart);
        jmethodID getFpsMethodID = env->GetMethodID(tsPartClass, "getFps", "()I");
        jmethodID getIndexMethodID = env->GetMethodID(tsPartClass, "getIndex", "()I");
        jmethodID getNumberOfFramesMethodID = env->GetMethodID(tsPartClass, "getNumberOfFrames", "()I");
        jmethodID getUrlMethodID = env->GetMethodID(tsPartClass, "getUrl", "()Ljava/lang/String;");
        const char *str = env->GetStringUTFChars(static_cast<jstring>(env->CallObjectMethod(tsPart, getUrlMethodID)), NULL);
        string url(str);

        Util::Log(Util::Log::Severity::Debugger) << url;

        tsParts.push_back(make_shared<TSPart>(
                env->CallIntMethod(tsPart, getIndexMethodID),
                url,
                env->CallIntMethod(tsPart, getNumberOfFramesMethodID),
                env->CallIntMethod(tsPart, getFpsMethodID)));
    }

    /*for (int i = 1; i < 42; i++) {
        sprintf(charBuffer, formatString, quality, chunkStep * i);
        string url(charBuffer);
        tsParts.push_back(make_shared<TSPart>(i, url, 180, 30));
    }*/

    sStream_ = new Stream(stateDelegateImpl, loadServiceImpl, tsParts);
    sStream_->start();
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_deleteStream(JNIEnv *env, jclass type) {
    sStream_->stop();
    delete sStream_;
}

JNIEXPORT jboolean JNICALL Java_com_example_tos_jni_JNIStream_bindFrame(JNIEnv *env, jclass type, jlong index, jint textureIDY, jint textureIDU, jint textureIDV) {
    auto frame = sStream_->getFrame(index);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, yPlane->getData());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDU);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width/2, height/2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, uPlane->getData());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureIDV);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width/2, height/2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, vPlane->getData());

    return true;
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_setData(JNIEnv *env, jclass type, jbyteArray data, jint part) {
    int length = env->GetArrayLength(data);

    void *dataBuffer = env->GetPrimitiveArrayCritical(data, NULL);
    RawData *rawData = new RawData(dataBuffer, length);

    env->ReleasePrimitiveArrayCritical(data, dataBuffer, 0);

    sStream_->setData(rawData, part);

    if (env->ExceptionCheck()) return;
}
