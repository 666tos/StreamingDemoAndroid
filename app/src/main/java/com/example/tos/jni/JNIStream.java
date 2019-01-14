package com.example.tos.jni;

import com.example.tos.stream.Loader;
import com.example.tos.stream.StateDelegate;
import com.example.tos.stream.TsPart;

import java.util.List;

public class JNIStream {
    static public native void createStream(List<TsPart> tsPartList, int tsPartListSize, StateDelegate stateDelegate, Loader loader);
    static public native void deleteStream();

    static public native boolean bindFrame(long index, int textureIDY, int textureIDU, int textureIDV, int uTextureAspectRatio);

    static public native void setData(byte data[], int part);

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }
}
