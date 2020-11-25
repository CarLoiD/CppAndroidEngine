package com.carloid.cppandroidengine;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

public class EngineGLSurfaceView extends GLSurfaceView {
    public EngineGLSurfaceView(final Context context, final int width, final int height) {
        super(context);

        setEGLContextClientVersion(2);
        getHolder().setFormat(PixelFormat.RGBA_8888);

        setRenderer(new EngineGLRenderer(width, height));
    }
}
