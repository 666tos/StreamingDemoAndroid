//
// Created by M Ivaniushchenko on 1/18/19.
//

#ifndef STREAMINGDEMOANDROID_JNIPLAYLISTCONVERTER_H
#define STREAMINGDEMOANDROID_JNIPLAYLISTCONVERTER_H

#include "Playlist.hpp"
#include "PlaylistItem.hpp"
#include "JNIUtils.h"

#include <jni.h>
#include <Public/Data/RawData.hpp>

namespace Core {
    class JNIPlaylistConverter {
    public:
        static StreamingEngine::Playlist *createPlaylist(JNIEnv *env, jobject tsPartList, jint tsPartListSize);

    private:
        static StreamingEngine::Playlist::ItemRef createPlaylistItem(JNIEnv *env, jobject tsPart);
        static StreamingEngine::Playlist::XKeyRef createKey(JNIEnv *env, jobject tsPartKey);
    };
}


#endif //STREAMINGDEMOANDROID_JNIPLAYLISTCONVERTER_H
