package com.example.tos.stream;

import com.example.tos.jni.JNIStream;

public class StreamingCore {

    final Loader loader = new Loader();

    public StreamingCore(StateDelegate stateDelegate) {
        JNIStream.createStream(stateDelegate, loader);
        loader.mStreamingCore = this;
    }

    public boolean bindFrame(long index, int uPlaneY, int uPlaneU, int uPlaneV) {
        return JNIStream.bindFrame(index, uPlaneY, uPlaneU, uPlaneV);
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
