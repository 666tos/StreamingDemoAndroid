package com.example.tos.streamingdemoandroid;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewTreeObserver;

import com.example.tos.stream.StateDelegate;
import com.example.tos.stream.StreamingCore;

public class MainActivity extends Activity implements Renderer.Delegate, StateDelegate {

    private GLSurfaceView mGLSurfaceView;

    final double VIDEO_FPS = 30;
    final double PLAYBACK_SPEED = 2.0;
    double startTime = 0;

    StreamingCore streamingCore_ = new StreamingCore(this);

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

        startTime = getTime();

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
        double currentTime = getTime();
        double frameIndex = (currentTime - startTime) * VIDEO_FPS * PLAYBACK_SPEED;

        streamingCore_.bindFrame((int)frameIndex, uPlaneY, uPlaneU, uPlaneV, uTextureAspectRatio);
    }

    // StateDelegate

    @Override
    public void stateChanged(int state) {
        final StreamState streamState = StreamState.fromInt(state);
    }

    private double getTime() {
        return (double) System.currentTimeMillis()/1000;
    }
}
