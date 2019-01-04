package com.example.tos.stream;

import com.example.tos.jni.JNIStream;

public class StreamingCore {

    public StreamingCore() {
        JNIStream.createStream();
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
