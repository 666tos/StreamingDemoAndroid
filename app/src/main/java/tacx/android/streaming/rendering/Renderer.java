package tacx.android.streaming.rendering;

import android.content.res.Resources;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.opengles.GL10;

import tacx.android.R;

public class Renderer implements GLSurfaceView.Renderer  {
    public interface Delegate {
        void bindFrame(int uPlaneY, int uPlaneU, int uPlaneV, int uTextureAspectRatio);
    }


    private final Resources mResources;
    private final Delegate mDelegate;

    private volatile float mScreenAspectRatio = 1;

    float[] mCubeVertexData = {
            1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1, 1,
            -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 1,
            1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1, 0,
            1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1, 0,
            -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 1,
            -1.0f, -1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0
    };

    private int shaderProgram;
    int[] vertexBuffer;
    int textureIDY;
    int textureIDU;
    int textureIDV;

    int inPosition;
    int inNormal;
    int inTexCoord;
    int uPlaneY;
    int uPlaneU;
    int uPlaneV;
    int uScreenAspectRatio;
    int uTextureAspectRatio;

    public Renderer(final Resources resources, final Delegate delegate) {
        mResources = resources;
        mDelegate = delegate;
    }

    @Override
    public void onSurfaceCreated(GL10 glUnused, javax.microedition.khronos.egl.EGLConfig config) {
        vertexBuffer = new int[1];
        GLES20.glGenBuffers(1, vertexBuffer, 0);
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vertexBuffer[0]);

        FloatBuffer floatBuffer = ByteBuffer.allocateDirect(mCubeVertexData.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        floatBuffer.put(mCubeVertexData).position(0);

        GLES20.glBufferData(GLES20.GL_ARRAY_BUFFER,mCubeVertexData.length * 4, floatBuffer, GLES20.GL_STATIC_DRAW);

        GLES20.glEnableVertexAttribArray(0);
        GLES20.glVertexAttribPointer(0, 3, GLES20.GL_FLOAT, false, 8*4, 0);

        GLES20.glEnableVertexAttribArray(1);
        GLES20.glVertexAttribPointer(1, 3, GLES20.GL_FLOAT, false, 8*4, 12);

        GLES20.glEnableVertexAttribArray(2);
        GLES20.glVertexAttribPointer(2, 2, GLES20.GL_FLOAT, false, 8*4, 24);

        final int vertexShaderHandle = ShaderHelper.compileShader(mResources, GLES20.GL_VERTEX_SHADER, R.raw.video_vs);
        final int fragmentShaderHandle = ShaderHelper.compileShader(mResources, GLES20.GL_FRAGMENT_SHADER, R.raw.video_fs);

        shaderProgram = ShaderHelper.createAndLinkProgram(vertexShaderHandle, fragmentShaderHandle,
                new String[] {"_inPosition",  "_inNormal", "_inTexCoord"});

        inPosition = GLES20.glGetAttribLocation(shaderProgram, "_inPosition");
        inNormal = GLES20.glGetAttribLocation(shaderProgram, "_inNormal");
        inTexCoord = GLES20.glGetAttribLocation(shaderProgram, "_inTexCoord");

        uPlaneY = GLES20.glGetUniformLocation(shaderProgram, "_uPlaneY");
        uPlaneU = GLES20.glGetUniformLocation(shaderProgram, "_uPlaneU");
        uPlaneV = GLES20.glGetUniformLocation(shaderProgram, "_uPlaneV");

        uScreenAspectRatio = GLES20.glGetUniformLocation(shaderProgram, "_uScreenAspectRatio");
        uTextureAspectRatio = GLES20.glGetUniformLocation(shaderProgram, "_uTextureAspectRatio");

        // Create the texture
        textureIDY = loadTexture();
        textureIDU = loadTexture();
        textureIDV = loadTexture();
    }

    private int loadTexture() {
        final int[] textureHandle = new int[1];

        GLES20.glGenTextures(1, textureHandle, 0);

        if (textureHandle[0] == 0) {
            throw new RuntimeException("Error generating texture name.");
        }

        // Bind to the texture in OpenGL
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

        // Set filtering
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);

        return textureHandle[0];
    }

    @Override
    public void onSurfaceChanged(GL10 glUnused, int width, int height) {
    }

    @Override
    public void onDrawFrame(GL10 glUnused) {
        GLES20.glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        GLES20.glUseProgram(shaderProgram);

        GLES20.glUniform1i(uPlaneY, 0); // Slot0 - GL_TEXTURE0
        GLES20.glUniform1i(uPlaneU, 1); // Slot1 - GL_TEXTURE0
        GLES20.glUniform1i(uPlaneV, 2); // Slot2 - GL_TEXTURE0

        GLES20.glUniform1f(uScreenAspectRatio, mScreenAspectRatio);

        mDelegate.bindFrame(textureIDY, textureIDU, textureIDV, uTextureAspectRatio);

        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, vertexBuffer[0]);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6);
        GLES20.glFinish();
    }

    public void setScreenAspectRatio(final float aspectRatio) {
        mScreenAspectRatio = aspectRatio;
    }

}