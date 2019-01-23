package tacx.android.streaming;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewTreeObserver;

import java.util.concurrent.TimeUnit;

import tacx.android.streaming.content.StateDelegate;
import tacx.android.streaming.content.StreamingCore;
import tacx.android.streaming.rendering.Renderer;

public class MainActivity extends Activity implements Renderer.Delegate, StateDelegate {

    private static final double PLAYBACK_SPEED = 1.2;
    private static final String PLAYLIST_URL = "https://tcdevmediaservice-euwe.streaming.media.azure.net/df014ada-d739-42d6-9517-18b131b17db7/T205541.ism/manifest(format=m3u8-aapl)";

    private final StreamingCore mStreamingCore = new StreamingCore(PLAYLIST_URL, this);

    private GLSurfaceView mGLSurfaceView;

    private static long mStartTime = -1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Renderer renderer = new Renderer(getResources(), this);
        mGLSurfaceView = new GLSurfaceView(this);
        mGLSurfaceView.setEGLContextClientVersion(2);
        mGLSurfaceView.setRenderer(renderer);

        mGLSurfaceView.getViewTreeObserver().addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {

            @Override
            public boolean onPreDraw() {
                mGLSurfaceView.getViewTreeObserver().removeOnPreDrawListener(this);

                renderer.setScreenAspectRatio((float) mGLSurfaceView.getWidth() / mGLSurfaceView.getHeight());

                return true;
            }

        });

        setContentView(mGLSurfaceView);
    }

    @Override
    protected void onResume()  {
        // The activity must call the GL surface view's onResume() on activity onResume().
        super.onResume();

        if (mStartTime == -1) {
            mStartTime = getTime();
        }

        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        // The activity must call the GL surface view's onPause() on activity onPause().
        super.onPause();
        mGLSurfaceView.onPause();
    }

    // Renderer.Delegate

    @Override
    public void bindFrame(int uPlaneY, int uPlaneU, int uPlaneV, int uTextureAspectRatio) {
        final long currentTime = getTime();
        final double seconds = (double) (currentTime - mStartTime) / 1000;
        final double frameTimestamp = seconds * PLAYBACK_SPEED;

        mStreamingCore.bindFrame(frameTimestamp, uPlaneY, uPlaneU, uPlaneV, uTextureAspectRatio);
    }

    // StateDelegate

    @Override
    public void stateChanged(int state) {
        final StreamState streamState = StreamState.fromInt(state);
    }

    private long getTime() {
        return TimeUnit.NANOSECONDS.toMillis(System.nanoTime());
    }

}