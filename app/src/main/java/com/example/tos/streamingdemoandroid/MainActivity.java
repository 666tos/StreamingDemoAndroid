package com.example.tos.streamingdemoandroid;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.TextView;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

import com.example.tos.jni.JNIStream;
import com.example.tos.stream.StreamingCore;

public class MainActivity extends Activity implements Renderer.Delegate {

    private GLSurfaceView mGLSurfaceView;

    static int index_ = 0;
    StreamingCore streamingCore_ = new StreamingCore();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Renderer renderer = new Renderer(this, this);
        mGLSurfaceView = new GLSurfaceView(this);
        mGLSurfaceView.setEGLContextClientVersion(2);
        mGLSurfaceView.setRenderer(renderer);

//        ViewGroup mainView = findViewById(R.layout.activity_main);
//        mainView.addView(mGLSurfaceView);

        setContentView(mGLSurfaceView);
    }

    @Override
    protected void onResume()  {
        // The activity must call the GL surface view's onResume() on activity onResume().
        super.onResume();
        mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        // The activity must call the GL surface view's onPause() on activity onPause().
        super.onPause();
        mGLSurfaceView.onPause();
    }

    @Override
    public void bindFrame() {
        index_++;

        streamingCore_.getFrame(index_);
    }
}
