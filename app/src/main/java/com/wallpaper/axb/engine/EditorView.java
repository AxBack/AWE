package com.wallpaper.axb.engine;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.support.v4.view.GestureDetectorCompat;
import android.support.v4.view.MotionEventCompat;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.SurfaceHolder;

/**
 * Created by AxB on 2/21/2018.
 */

public class EditorView extends GLSurfaceView implements
        ScaleGestureDetector.OnScaleGestureListener,
        GestureDetector.OnGestureListener {

    private static boolean DEBUG = true;

    private com.wallpaper.axb.engine.Renderer mRenderer;

    private ScaleGestureDetector mScaleGestureDetector;
    private GestureDetectorCompat mGestureDetector;

    private float mOffset = 0;
    private float mTouchX = 0;
    private float mTouchY = 0;

    public EditorView(Context context) {
        super(context);

        init(context);
    }

    public EditorView(Context context, AttributeSet attrs) {
        super(context, attrs);

        init(context);
    }

    private void init(Context context) {
        setEGLConfigChooser(8, 8, 8, 8, 24, 1);
        setEGLContextClientVersion(3);
        setPreserveEGLContextOnPause(true);

        if (DEBUG) {
            setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR);
        }

        mRenderer = new com.wallpaper.axb.engine.Renderer(context, "temp.dat");
        setRenderer(mRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);

        mScaleGestureDetector = new ScaleGestureDetector(context, this);
        mGestureDetector = new GestureDetectorCompat(context, this);
    }

    public void reset() {
        mRenderer.reset("temp.dat");
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        super.surfaceCreated(holder);
    }

    @Override
    public void onResume() {
        super.onResume();
        mRenderer.resume();
    }

    @Override
    public void onPause() {
        super.onPause();
        mRenderer.pause();
    }

    public void onDestroy() {
        super.onDetachedFromWindow();

        mRenderer.destroy();
        mRenderer = null;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector scaleGestureDetector) {
        return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector scaleGestureDetector) {
    }

    @Override
    public boolean onScale(ScaleGestureDetector scaleGestureDetector) {
        float diff = 1.0f - scaleGestureDetector.getScaleFactor();
        if(diff != 0.0f)
            mRenderer.onPinch(diff);
        return true;
    }

    @Override
    public boolean onFling(MotionEvent motionEvent, MotionEvent motionEvent1, float v, float v1) {
        return false;
    }

    @Override
    public boolean onSingleTapUp(MotionEvent motionEvent) {
        return false;
    }

    @Override
    public void onLongPress(MotionEvent motionEvent) {
        //temp
    }

    @Override
    public boolean onDown(MotionEvent motionEvent) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent motionEvent) {

    }

    @Override
    public boolean onScroll(MotionEvent motionEvent, MotionEvent motionEvent1, float v, float v1) {
        v = v / (float)getWidth();
        mOffset = Math.max(Math.min(mOffset + v, 1.0f), 0.0f);
        mRenderer.onOffsetChanged(mOffset, 0); // no working
        return true;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        mScaleGestureDetector.onTouchEvent(event);
        mGestureDetector.onTouchEvent(event);
        //mRotationDetector.onTouchEvent(event);

        final float x = event.getX();
        final float y = event.getY();

        if (x != mTouchX || y != mTouchY) {
            mTouchX = x;
            mTouchY = y;

            if (event.getAction() == MotionEvent.ACTION_DOWN
                    || event.getAction() == MotionEvent.ACTION_MOVE) {
                queueEvent(() -> mRenderer.onTouch(x, y));
            }
        }
        return true;
    }
}
