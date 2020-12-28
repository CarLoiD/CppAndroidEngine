package com.carloid.cppandroidengine;

import android.graphics.Point;
import android.os.Bundle;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

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


    // TouchScreen
    private static float mCursorAxisX = -1.0f;
    private static float mCursorAxisY = -1.0f;
    private static float mMultiCursorAxisX = -1.0f;
    private static float mMultiCursorAxisY = -1.0f;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(UI_FLAGS);

        decorView.setOnSystemUiVisibilityChangeListener(visibility ->
                decorView.setSystemUiVisibility(UI_FLAGS)
        );

        final Display tmpDisplay = getWindowManager().getDefaultDisplay();

        Point tmpSize = new Point();
        tmpDisplay.getRealSize(tmpSize);

        EngineGLSurfaceView mSurfaceView = new EngineGLSurfaceView(this, tmpSize.x, tmpSize.y);
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

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN ||
            event.getAction() == MotionEvent.ACTION_MOVE)
        {
            // Single touch event
            mCursorAxisX = event.getX(0);
            mCursorAxisY = event.getY(0);

            // Multi touch event
            if (event.getPointerCount() > 1) {
                mMultiCursorAxisX = event.getX(1);
                mMultiCursorAxisY = event.getY(1);
            }
        } else {
            mCursorAxisX = -1.0f;
            mCursorAxisY = -1.0f;
            mMultiCursorAxisX = -1.0f;
            mMultiCursorAxisY = -1.0f;
        }

        return true;
    }

    private native void ApplicationDestroy();
}