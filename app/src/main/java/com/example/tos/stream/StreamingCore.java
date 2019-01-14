package com.example.tos.stream;

import com.example.tos.jni.JNIStream;

import java.util.List;

public class StreamingCore {

    final Loader loader = new Loader();

    public StreamingCore(StateDelegate stateDelegate) {
        List<TsPart> tsPartList = null;
        final String playlistUrl = "https://tcdevmediaservice-euwe.streaming.media.azure.net/26c5c622-eeb9-43d8-8885-bbe8b12f2885/Stelvio.ism/manifest(format=m3u8-aapl)";
        final int targetBitrate = 1000000;
        try {
            tsPartList = new PlaylistParser(playlistUrl, targetBitrate).execute().get();
        } catch (Exception ignore) {
        }

        if (tsPartList != null) {
            JNIStream.createStream(tsPartList, tsPartList.size(), stateDelegate, loader);
            loader.mStreamingCore = this;
        }
    }

    public boolean bindFrame(long index, int uPlaneY, int uPlaneU, int uPlaneV, int uTextureAspectRatio) {
        return JNIStream.bindFrame(index, uPlaneY, uPlaneU, uPlaneV, uTextureAspectRatio);
    }

    public void setData(byte data[], int part) {
        JNIStream.setData(data, part);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();

        JNIStream.deleteStream();
    }
}
