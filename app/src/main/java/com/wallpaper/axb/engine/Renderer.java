package com.wallpaper.axb.engine;

import android.content.Context;
import android.content.SharedPreferences;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.content.Context.MODE_PRIVATE;

class Renderer implements GLSurfaceView.Renderer {

    private final Context mContext;
    private final NativeEngine mRenderEngine = new NativeEngine();

    private int mNativeId = -1;
    private boolean mIsActive = false;

    private int mWidth = 0;
    private int mHeight = 0;

    public Renderer(Context ctx) {
        mContext = ctx;
    }

    public synchronized void restart() {
        if(mNativeId >= 0)
            mRenderEngine.restart(mNativeId);
    }

    public synchronized void destroy() {
        if(mNativeId >= 0)
            mRenderEngine.destroy(mNativeId);
        mNativeId = -1;
    }

    @Override
    public synchronized void onSurfaceCreated(GL10 gl, EGLConfig config) {
        createEngine();
    }

    @Override
    public synchronized void onSurfaceChanged(GL10 gl, int width, int height) {
        mWidth = width;
        mHeight = height;
        if(mNativeId >= 0)
            mRenderEngine.setSize(mNativeId, width,height);
    }

    @Override
    public synchronized void onDrawFrame(GL10 gl) {
        if(mNativeId >= 0)
            mRenderEngine.render(mNativeId);
    }

    public void onOffsetChanged(float xOffset, float yOffset) {
        if (mNativeId >= 0)
            mRenderEngine.onOffsetChanged(mNativeId, xOffset, yOffset);
    }

    public void resume() {
        if(!mIsActive) {
            mIsActive = true;
            if(mNativeId >= 0)
                mRenderEngine.resume(mNativeId);
        }
    }

    public void pause() {
        if(mIsActive) {
            mIsActive = false;
            if(mNativeId >= 0)
                mRenderEngine.pause(mNativeId);
        }
    }

    public void onTouch(float x, float y) {
        if(mNativeId >= 0) {
            mRenderEngine.onTouch(mNativeId,x,y);
        }
    }

    public void onPinch(float diff) {
        if(mNativeId >= 0) {
            mRenderEngine.onPinch(mNativeId, diff);
        }
    }

    public void onRotation(float angle) {
        if(mNativeId >= 0) {
            mRenderEngine.onRotation(mNativeId, angle);
        }
    }

    private synchronized void createEngine() {
        if(mNativeId > -1)
            return;

        mNativeId = mRenderEngine.create("Electric", mContext.getFilesDir().getAbsolutePath(), mContext.getAssets());
        if(mIsActive)
            mRenderEngine.resume(mNativeId);

    }
}
