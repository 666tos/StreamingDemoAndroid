//
// Created by M Ivaniushchenko on 1/18/19.
//

#include "JNIPlaylistConverter.h"

#include <iosfwd>

using namespace std;
using namespace Core;
using namespace StreamingEngine;

// FIXME: do it in more elegant way
const char * tsPartKeyClassName = "tacx/android/streaming/content/TsPartKey";

Playlist *JNIPlaylistConverter::createPlaylist(JNIEnv *env, jobject tsPartList, jint tsPartListSize) {
    vector<StreamingEngine::Playlist::ItemRef> playlistItems;

    for (int i = 0; i < tsPartListSize; i++) {
        jclass listClass = env->GetObjectClass(tsPartList);
        jmethodID getMethodID = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

        jobject tsPart = env->CallObjectMethod(tsPartList, getMethodID, i);

        auto playlistItem = createPlaylistItem(env, tsPart);

        playlistItems.push_back(playlistItem);
    }

    return new Playlist(playlistItems);
}

Playlist::ItemRef JNIPlaylistConverter::createPlaylistItem(JNIEnv *env, jobject tsPart) {
    jclass tsPartClass = env->GetObjectClass(tsPart);
    jmethodID getDurationMethodID = env->GetMethodID(tsPartClass, "getDuration", "()D");
    jmethodID getIndexMethodID = env->GetMethodID(tsPartClass, "getIndex", "()I");
    jmethodID getUrlMethodID = env->GetMethodID(tsPartClass, "getUrl", "()Ljava/lang/String;");

    std::stringstream ss;
    ss << "()L" << tsPartKeyClassName << ";";

    jmethodID getEncryptionKeyMethodID = env->GetMethodID(tsPartClass, "getEncryptionKey", ss.str().c_str());

    //env->R

    int64_t tag = env->CallIntMethod(tsPart, getIndexMethodID);
    auto url = JNIUtils::callStringMethod(env, tsPart, getUrlMethodID);
    double duration = env->CallDoubleMethod(tsPart, getDurationMethodID);
    jobject tsPartKey = env->CallObjectMethod(tsPart, getEncryptionKeyMethodID);
    auto xKey = createKey(env, tsPartKey);

    auto playlistItem = make_shared<Playlist::Item>(xKey, tag, url, duration);
    return playlistItem;
}

Playlist::XKeyRef JNIPlaylistConverter::createKey(JNIEnv *env, jobject tsPartKey) {
    jclass tsPartKeyClass = env->GetObjectClass(tsPartKey);
    jmethodID getMethodMethodID = env->GetMethodID(tsPartKeyClass, "getMethod", "()Ljava/lang/String;");
    jmethodID getUriMethodID = env->GetMethodID(tsPartKeyClass, "getUri", "()Ljava/lang/String;");
    jmethodID getIVMethodID = env->GetMethodID(tsPartKeyClass, "getIV", "()[B");
    jmethodID getKeyFormatMethodID = env->GetMethodID(tsPartKeyClass, "getKeyFormat", "()Ljava/lang/String;");
    jmethodID getKeyFormatVersionMethodID = env->GetMethodID(tsPartKeyClass, "getKeyFormatVersions", "()Ljava/lang/String;");

    auto method = JNIUtils::callStringMethod(env, tsPartKey, getMethodMethodID);
    auto uri = JNIUtils::callStringMethod(env, tsPartKey, getUriMethodID);
    auto keyFormat = JNIUtils::callStringMethod(env, tsPartKey, getKeyFormatMethodID);
    auto keyFormatVersions = JNIUtils::callStringMethod(env, tsPartKey, getKeyFormatVersionMethodID);

    jbyteArray jdata = (jbyteArray)env->CallObjectMethod(tsPartKey, getIVMethodID);
    auto ivData = JNIUtils::rawDataFromByteArray(env, jdata);

    return std::make_shared<Playlist::XKey>(method, uri, ivData, keyFormat, keyFormatVersions);
}