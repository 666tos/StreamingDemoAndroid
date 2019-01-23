package tacx.android.streaming.content;

import android.os.AsyncTask;

import java.util.List;

import tacx.android.streaming.jni.JNIStream;

public class StreamingCore {

    private final long mNativeStreamHandle;

    private static class DisposeTask extends AsyncTask<Void, Void, Void> {

        private final long mNativeStreamHandle;

        private DisposeTask(final long nativeStreamHandle) {
            mNativeStreamHandle = nativeStreamHandle;
        }

        @Override
        protected Void doInBackground(final Void... voids) {
            JNIStream.deleteStream(mNativeStreamHandle);

            return null;
        }

    }

    public StreamingCore(final String playlistUrl, final StateDelegate stateDelegate) {
        List<TsPart> tsPartList = null;
        final int targetBitrate = 1000000;
        try {
            tsPartList = new PlaylistParser(playlistUrl, targetBitrate).execute().get();
        } catch (Exception ignore) {
        }

        if (tsPartList != null) {
            final ContentLoader contentLoader = new ContentLoader(this);
            mNativeStreamHandle = JNIStream.createStream(tsPartList, tsPartList.size(), stateDelegate, contentLoader);
        } else {
            mNativeStreamHandle = -1;
        }
    }

    public boolean bindFrame(double timestamp, int uPlaneY, int uPlaneU, int uPlaneV, int uTextureAspectRatio) {
        return JNIStream.bindFrame(mNativeStreamHandle, timestamp, uPlaneY, uPlaneU, uPlaneV, uTextureAspectRatio);
    }

    public void dispose() {
        new DisposeTask(mNativeStreamHandle).execute();
    }

    public void setData(byte data[], int part) {
        JNIStream.setData(mNativeStreamHandle, data, part);
    }

    public void setDecryptionKeyData(byte data[], String url) {
        JNIStream.setDecryptionKeyData(mNativeStreamHandle, data, url);
    }

}