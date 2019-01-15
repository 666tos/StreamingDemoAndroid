package com.example.tos.streamingdemoandroid;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewTreeObserver;

import com.example.tos.stream.StateDelegate;
import com.example.tos.stream.StreamingCore;

import java.util.concurrent.TimeUnit;

public class MainActivity extends Activity implements Renderer.Delegate, StateDelegate {

    private static final double PLAYBACK_SPEED = 1.2;

    private final StreamingCore mStreamingCore = new StreamingCore(this);

    private GLSurfaceView mGLSurfaceView;
    private long mStartTime = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Renderer renderer = new Renderer(this, this);
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

        mStartTime = getTime();

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