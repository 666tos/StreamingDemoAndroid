package tacx.android.streaming.content;

import android.os.AsyncTask;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.lang.ref.WeakReference;
import java.net.URL;
import java.net.URLConnection;

public class ContentLoader {

    private static class ContentLoaderTask extends AsyncTask<Void, Void, Void> {

        private final WeakReference<StreamingCore> mStreamingCoreRef;
        private final int mTag;
        private final String mUrlString;

        ContentLoaderTask(final WeakReference<StreamingCore> streamingCoreRef, final int tag, final String urlString) {
            mStreamingCoreRef = streamingCoreRef;
            mTag = tag;
            mUrlString = urlString;
        }

        @Override
        protected Void doInBackground(Void... params) {
            try {
                final URL url = new URL(mUrlString);
                final URLConnection connection = url.openConnection();
                connection.connect();

                // this will be useful so that you can show a tipical 0-100%
                // progress bar
                final int contentLength = connection.getContentLength();

                // download the file
                final InputStream inputStream = connection.getInputStream();
                final ByteArrayOutputStream buffer = new ByteArrayOutputStream();

                int nRead;
                final byte[] bufferData = new byte[16384];

                while ((nRead = inputStream.read(bufferData, 0, bufferData.length)) != -1) {
                    buffer.write(bufferData, 0, nRead);
                }

                final byte data[] = buffer.toByteArray();
                buffer.close();

                // closing streams
                inputStream.close();

                final StreamingCore streamingCore = mStreamingCoreRef.get();
                if (streamingCore != null) {
                    streamingCore.setData(data, mTag);
                }
            } catch (Exception e) {
                Log.e("Error: ", e.getMessage());
            }

            return null;
        }

    }

    private static class KeyLoaderTask extends AsyncTask<Void, Void, Void> {

        private final WeakReference<StreamingCore> mStreamingCoreRef;
        private final String mUrlString;

        KeyLoaderTask(final WeakReference<StreamingCore> streamingCoreRef, final String urlString) {
            mStreamingCoreRef = streamingCoreRef;
            mUrlString = urlString;
        }

        @Override
        protected Void doInBackground(Void... params) {
            try {
                final URL url = new URL(mUrlString);
                final URLConnection connection = url.openConnection();
                connection.connect();

                // this will be useful so that you can show a tipical 0-100%
                // progress bar
                final int contentLength = connection.getContentLength();

                // download the file
                final InputStream inputStream = connection.getInputStream();
                final ByteArrayOutputStream buffer = new ByteArrayOutputStream();

                int nRead;
                final byte[] bufferData = new byte[16384];

                while ((nRead = inputStream.read(bufferData, 0, bufferData.length)) != -1) {
                    buffer.write(bufferData, 0, nRead);
                }

                final byte data[] = buffer.toByteArray();
                buffer.close();

                // closing streams
                inputStream.close();

                final StreamingCore streamingCore = mStreamingCoreRef.get();
                if (streamingCore != null) {
                    streamingCore.setDecryptionKeyData(data, mUrlString);
                }
            } catch (Exception e) {
                Log.e("Error: ", e.getMessage());
            }

            return null;
        }

    }

    private final WeakReference<StreamingCore> mStreamingCoreRef;

    private AsyncTask<Void, Void, Void> mContentTask;
    private AsyncTask<Void, Void, Void> mKeyTask;

    ContentLoader(final StreamingCore streamingCore) {
        mStreamingCoreRef = new WeakReference<>(streamingCore);
    }

    public void load(final String urlString, final int tag) {
        System.out.println("Load chunk: tag: " + tag + " url: " + urlString);

        mContentTask = new ContentLoaderTask(mStreamingCoreRef, tag, urlString);
        mContentTask.execute();
    }

    public void loadKey(final String urlString) {
        System.out.println("Load decryption key: url: " + urlString);

        mKeyTask = new KeyLoaderTask(mStreamingCoreRef, urlString);
        mKeyTask.execute();
    }

}