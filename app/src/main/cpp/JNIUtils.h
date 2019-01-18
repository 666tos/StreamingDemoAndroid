//
// Created by M Ivaniushchenko on 1/18/19.
//

#ifndef STREAMINGDEMOANDROID_JNIUTILS_H
#define STREAMINGDEMOANDROID_JNIUTILS_H

#import "RawData.hpp"

#import <jni.h>

namespace Core {

    class JNIUtils {
    public:
        static StreamingEngine::RawDataRef rawDataFromByteArray(JNIEnv *jenv, jbyteArray jdata);
        static std::string string(JNIEnv *jenv, jstring jstring);
        static std::string callStringMethod(JNIEnv *jenv, jobject jObject, jmethodID jMethodID);
    };

}

#endif //STREAMINGDEMOANDROID_JNIUTILS_H
