package com.carloid.cppandroidengine;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("CppAndroidEngineJNI");
    }

    private static final int UI_FLAGS = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                      | View.SYSTEM_UI_FLAG_FULLSCREEN
                                      | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                                      | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                      | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                      | View.SYSTEM_UI_FLAG_LAYOUT_STABLE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(UI_FLAGS);

        decorView.setOnSystemUiVisibilityChangeListener(visibility ->
                decorView.setSystemUiVisibility(UI_FLAGS)
        );

        EngineGLSurfaceView mSurfaceView = new EngineGLSurfaceView(this);
        setContentView(mSurfaceView);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        getWindow().getDecorView().setSystemUiVisibility(UI_FLAGS);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        // Nothing to do
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ApplicationDestroy();
    }

    private native void ApplicationDestroy();
}