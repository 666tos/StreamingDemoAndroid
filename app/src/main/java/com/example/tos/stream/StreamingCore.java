package com.example.tos.stream;

import com.example.tos.jni.JNIStream;

public class StreamingCore {

    final Loader loader = new Loader();

    public StreamingCore() {
        JNIStream.createStream(loader);
    }

    public boolean getFrame(long index) {
        return JNIStream.getFrame(index);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();

        JNIStream.deleteStream();
    }
}
