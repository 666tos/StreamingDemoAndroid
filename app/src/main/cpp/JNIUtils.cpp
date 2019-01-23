//
// Created by M Ivaniushchenko on 1/18/19.
//

#include "JNIUtils.h"

using namespace std;
using namespace Core;
using namespace StreamingEngine;

RawDataRef JNIUtils::rawDataFromByteArray(JNIEnv *jenv, jbyteArray jdata) {
    int length = jenv->GetArrayLength(jdata);

    void *dataBuffer = jenv->GetPrimitiveArrayCritical(jdata, NULL);
    auto rawData = make_shared<RawData>(dataBuffer, length);

    jenv->ReleasePrimitiveArrayCritical(jdata, dataBuffer, 0);

    return rawData;
}

string JNIUtils::string(JNIEnv *jenv, jstring jstring) {
    const char *chars = jenv->GetStringUTFChars(jstring, NULL);
    std::string stdString = std::string(chars);
    jenv->ReleaseStringUTFChars(jstring, chars);

    return stdString;
}

string JNIUtils::callStringMethod(JNIEnv *jenv, jobject jObject, jmethodID jMethodID) {
    return JNIUtils::string(jenv, (jstring)jenv->CallObjectMethod(jObject, jMethodID));
}