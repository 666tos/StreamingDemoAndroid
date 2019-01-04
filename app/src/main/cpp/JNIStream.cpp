//
// Created by M Ivaniushchenko on 1/4/19.
//

#include "JNIStream.h"
#include "Stream.hpp"
#include "IStreamStateDelegate.hpp"
#include "ITSPartLoaderService.hpp"
#include "Log.hpp"

#include <jni.h>
#include <sys/types.h>
#include <vector>

using namespace std;
using namespace StreamingEngine;

class StreamStateDelegateImpl: public IStreamStateDelegate {
public:
    virtual ~StreamStateDelegateImpl() {};
    virtual void streamStateChanged(StreamState state) {};
};

class TSPartLoaderServiceImpl: public ITSPartLoaderService {
public:
    virtual ~TSPartLoaderServiceImpl() {};
    virtual void load(TSPartRef tsPart) {
//        jclass localStreamClass = env->FindClass("java/io/InputStream");
//
//        if(NULL == localStreamClass)
//        {
//        }
//
//        streamClass_ = static_cast<jclass>(env->NewGlobalRef(localStreamClass));
        Util::Log(Util::Log::Severity::Verbose) << "Load: " << tsPart->tag() << " URL: " << tsPart->url();
    };
};

static Stream *sStream_ = nullptr;

JNIEXPORT void JNICALL Java_com_example_tos_jni_JNIStream_createStream(JNIEnv *env, jclass type) {
    auto stateDelegate = new StreamStateDelegateImpl();
    auto loadService = new TSPartLoaderServiceImpl();

    std::vector<TSPartRef> tsParts;

    char charBuffer[1024];
    const char *formatString = "https://nikitasplendo1-euwe.streaming.media.azure.net/125d908d-0b7f-42df-b8c2-cfb956ff37e8/T205523.ism/QualityLevels(2210000)/Fragments(video=%i0000000,format=m3u8-aapl-v3,audiotrack=aac_und_2_127_2_1)";

    for (int i = 1; i < 8; i++) {
        sprintf(charBuffer, formatString, i);
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
    return (frame != nullptr);
}