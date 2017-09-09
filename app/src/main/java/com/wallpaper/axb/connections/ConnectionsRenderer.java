package com.wallpaper.axb.connections;

import android.content.res.AssetManager;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class ConnectionsRenderer implements GLSurfaceView.Renderer {

    private final AssetManager mAssetManager;
    private final NativeEngine mRenderEngine = new NativeEngine();

    private int mNativeId = -1;
    private boolean mIsActive = false;

    public ConnectionsRenderer(AssetManager assetManager, Rect frame) {
        mAssetManager = assetManager;
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
        if(mNativeId >= 0)
            mRenderEngine.setSize(mNativeId, width,height);
    }

    @Override
    public synchronized void onDrawFrame(GL10 gl) {
        if(mNativeId >= 0)
            mRenderEngine.render(mNativeId);
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

    private synchronized void createEngine() {
        if(mNativeId > -1)
            return;

        mNativeId = mRenderEngine.create(mAssetManager);
        if(mIsActive)
            mRenderEngine.resume(mNativeId);

    }
}
