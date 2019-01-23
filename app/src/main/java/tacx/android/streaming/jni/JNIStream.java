package tacx.android.streaming.jni;

import java.util.List;

import tacx.android.streaming.content.ContentLoader;
import tacx.android.streaming.content.StateDelegate;
import tacx.android.streaming.content.TsPart;

public class JNIStream {

    // Used to load the 'TacxStreaming' library on application startup.
    static {
        System.loadLibrary("TacxStreaming");
    }

    public static native boolean bindFrame(long handle, double timestamp, int textureIDY, int textureIDU, int textureIDV, int uTextureAspectRatio);

    public static native long createStream(List<TsPart> tsPartList, int tsPartListSize, StateDelegate stateDelegate, ContentLoader loader);

    public static native void deleteStream(long handle);

    public static native void setData(long handle, byte data[], int part);

    public static native void setDecryptionKeyData(long handle, byte data[], String url);

}