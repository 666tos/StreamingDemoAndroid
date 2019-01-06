//
// Created by M Ivaniushchenko on 1/4/19.
//

#include "JNIStream.h"
#include "Stream.hpp"
#include "IStreamStateDelegate.hpp"
#include "ITSPartLoaderService.hpp"
#include "Log.hpp"
#include "RawData.hpp"
#include "JNICore.h"

#include <jni.h>
#include <sys/types.h>
#include <vector>
#include <GLES2/gl2.h>

using namespace std;
using namespace StreamingEngine;

class StreamStateDelegateImpl: public IStreamStateDelegate {
public:
    virtual ~StreamStateDelegateImpl() {}
    virtual void streamStateChanged(StreamState state) {}
};

class TSPartLoaderServiceImpl: public ITSPartLoaderService {
public:
    TSPartLoaderServiceImpl(jobject loader) {
        auto env = Core::JNICore::getJNIEnv();
        loader_= env->NewGlobalRef(loader);
    }

    virtual ~TSPartLoaderServiceImpl() {
        auto env = Core::JNICore::getJNIEnv();
        env->DeleteGlobalRef(loader_);
    }

    virtual void load(TSPartRef tsPart) {
        auto env = Core::JNICore::getJNIEnv();

        jclass loaderClass = env->GetObjectClass(loader_);
        jmethodID methodID = env->GetMethodID(loaderClass, "load", "(Ljava/lang/String;I)V");

        jstring url = env->NewStringUTF(tsPart->url().c_str());
        env->CallVoidMethod(loader_, methodID, url, tsPart->tag());

        Util::Log(Util::Log::Severity::Verbose) << "Load: " << tsPart->tag() << " URL: " << tsPart->url();
    }
private:
    jobject loader_;
};

static Stream *sStream_ = nullptr;

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type, jobject loader) {
    auto stateDelegate = new StreamStateDelegateImpl();
    auto loadService = new TSPartLoaderServiceImpl(loader);

    std::vector<TSPartRef> tsParts;

    char charBuffer[1024];
    const char *formatString = "https://nikitasplendo1-euwe.streaming.media.azure.net/125d908d-0b7f-42df-b8c2-cfb956ff37e8/T205523.ism/QualityLevels(720000)/Fragments(video=%i0000000,format=m3u8-aapl-v3,audiotrack=aac_und_2_127_2_1)";

    for (int i = 1; i < 8; i++) {
        sprintf(charBuffer, formatString, 6 * i);
        string url(charBuffer);
        tsParts.push_back(make_shared<TSPart>(i, url, 180, 30));
    }

    sStream_ = new Stream(stateDelegate, loadService, tsParts);
    sStream_->start();
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_deleteStream(JNIEnv *env, jclass type) {
    sStream_->stop();
    delete sStream_;
}

JNIEXPORT jboolean JNICALL Java_com_example_tos_jni_JNIStream_getFrame(JNIEnv *env, jclass type, jlong index) {
    auto frame = sStream_->getFrame(index);

    if (frame != nullptr) {
        auto yPlane = frame->getPlane(Frame::Plane::Y);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, frame->getWidth(), frame->getHeight(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, yPlane->getData());
    }

    return (frame != nullptr);
}

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_setData(JNIEnv *env, jclass type, jbyteArray data, jint part) {
    int length = env->GetArrayLength(data);

    void *dataBuffer = env->GetPrimitiveArrayCritical(data, NULL);
    RawData *rawData = new RawData(dataBuffer, length);

    env->ReleasePrimitiveArrayCritical(data, dataBuffer, 0);

    sStream_->setData(rawData, part);

    if (env->ExceptionCheck()) return;
}