package com.example.tos.jni;

public class JNIStream {
    /*
     * jni function declarations
     */
    static public native void createStream();
    static public native void deleteStream();

    static public native boolean getFrame(long index);

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }
}
