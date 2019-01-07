package com.example.tos.jni;

import com.example.tos.stream.Loader;
import com.example.tos.stream.StateDelegate;

public class JNIStream {
    static public native void createStream(StateDelegate stateDelegate, Loader loader);
    static public native void deleteStream();

    static public native boolean bindFrame(long index, int textureIDY, int textureIDU, int textureIDV);

    static public native void setData(byte data[], int part);

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }
}
