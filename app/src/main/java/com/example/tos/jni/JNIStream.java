package com.example.tos.jni;

import com.example.tos.stream.Loader;

public class JNIStream {
    /*
     * jni function declarations
     */
    static public native void createStream(Loader loader);
    static public native void deleteStream();

    static public native boolean getFrame(long index);

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }
}
