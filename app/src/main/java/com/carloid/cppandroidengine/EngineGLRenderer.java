package com.carloid.cppandroidengine;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class EngineGLRenderer implements GLSurfaceView.Renderer {

    private int mWidth;
    private int mHeight;

    EngineGLRenderer(final int width, final int height)
    {
        mWidth  = width;
        mHeight = height;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        ApplicationCreate(mWidth, mHeight);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        gl10.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        ApplicationUpdate();
    }

    private native void ApplicationCreate(final int width, final int height);
    private native void ApplicationUpdate();
}
