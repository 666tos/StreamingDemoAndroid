package com.example.tos.stream;

import com.example.tos.jni.JNIStream;

import java.util.List;

public class StreamingCore {

    private final Loader loader = new Loader();
    private final long nativeStreamHandle;

    public StreamingCore(StateDelegate stateDelegate) {
        List<TsPart> tsPartList = null;
        final String playlistUrl = "https://tcdevmediaservice-euwe.streaming.media.azure.net/df014ada-d739-42d6-9517-18b131b17db7/T205541.ism/manifest(format=m3u8-aapl)";
        final int targetBitrate = 1000000;
        try {
            tsPartList = new PlaylistParser(playlistUrl, targetBitrate).execute().get();
        } catch (Exception ignore) {
        }

        if (tsPartList != null) {
            nativeStreamHandle = JNIStream.createStream(tsPartList, tsPartList.size(), stateDelegate, loader);
            loader.mStreamingCore = this;
        }
        else {
            nativeStreamHandle = -1;
        }
    }

    public boolean bindFrame(double timestamp, int uPlaneY, int uPlaneU, int uPlaneV, int uTextureAspectRatio) {
        return JNIStream.bindFrame(nativeStreamHandle, timestamp, uPlaneY, uPlaneU, uPlaneV, uTextureAspectRatio);
    }

    public void setData(byte data[], int part) {
        JNIStream.setData(nativeStreamHandle, data, part);
    }

    public void setDecryptionKeyData(byte data[], String url) {
        JNIStream.setDecryptionKeyData(nativeStreamHandle, data, url);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();

        JNIStream.deleteStream(nativeStreamHandle);
    }
}
