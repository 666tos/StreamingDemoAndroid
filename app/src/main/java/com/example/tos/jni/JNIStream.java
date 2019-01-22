package com.example.tos.jni;

import com.example.tos.stream.Loader;
import com.example.tos.stream.StateDelegate;
import com.example.tos.stream.TsPart;

import java.util.List;

public class JNIStream {
    static public native long createStream(List<TsPart> tsPartList, int tsPartListSize, StateDelegate stateDelegate, Loader loader);
    static public native void deleteStream(long handle);

    static public native boolean bindFrame(long handle, double timestamp, int textureIDY, int textureIDU, int textureIDV, int uTextureAspectRatio);

    static public native void setData(long handle, byte data[], int part);

    static public native void setDecryptionKeyData(long handle, byte data[], String url);

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }
}
