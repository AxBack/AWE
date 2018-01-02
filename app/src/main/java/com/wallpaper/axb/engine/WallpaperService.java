package com.wallpaper.axb.engine;

import android.content.Context;
import android.content.SharedPreferences;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class WallpaperService extends android.service.wallpaper.WallpaperService {

    private Engine mEngine;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public android.service.wallpaper.WallpaperService.Engine onCreateEngine() {
        mEngine = new Engine();
        return mEngine;
    }

    private class Engine extends android.service.wallpaper.WallpaperService.Engine {

        private WallpaperView mSurfaceView;
        private Renderer mRenderer;

        private float mTouchX = 0;
        private float mTouchY = 0;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);

            mSurfaceView = new WallpaperView(WallpaperService.this);
            mSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 1);
            mSurfaceView.setEGLContextClientVersion(3);
            mSurfaceView.setPreserveEGLContextOnPause(true);
            mSurfaceView.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR);

            mRenderer = new Renderer(WallpaperService.this);
            mSurfaceView.setRenderer(mRenderer);
            mSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        }

        @Override
        public void onDestroy() {
            super.onDestroy();
            mSurfaceView.onDestroy();
            mRenderer.destroy();

            mSurfaceView = null;
            mRenderer = null;
        }

        @Override
        public void onSurfaceRedrawNeeded(SurfaceHolder holder) {
            mSurfaceView.requestRender();
            super.onSurfaceRedrawNeeded(holder);
        }

        @Override
        public void onVisibilityChanged(boolean visible) {
            super.onVisibilityChanged(visible);

            if (mSurfaceView != null) {
                if (visible)
                    mSurfaceView.onResume();
                else
                    mSurfaceView.onPause();
            }
        }

        @Override
        public void onOffsetsChanged(float xOffset, float yOffset, float xOffsetStep, float yOffsetStep, int xPixelOffset, int yPixelOffset) {
            super.onOffsetsChanged(xOffset, yOffset, xOffsetStep, yOffsetStep, xPixelOffset, yPixelOffset);
            mRenderer.onOffsetChanged(xOffset, yOffset);
        }

        @Override
        public void onTouchEvent(MotionEvent event) {
            super.onTouchEvent(event);

            final float x = event.getX();
            final float y = event.getY();

            if (x != mTouchX || y != mTouchY) {
                mTouchX = x;
                mTouchY = y;

                if (event.getAction() == MotionEvent.ACTION_DOWN
                        || event.getAction() == MotionEvent.ACTION_MOVE) {
                    mSurfaceView.queueEvent(new Runnable() {
                        @Override
                        public void run() {
                            mRenderer.onTouch(x, y);
                        }
                    });
                }
            }
        }

        private class WallpaperView extends GLSurfaceView {

            public WallpaperView(Context context) {
                super(context);
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

            @Override
            public SurfaceHolder getHolder() {
                return mEngine.getSurfaceHolder();
            }

            public void onDestroy() {
                super.onDetachedFromWindow();
            }
        }
    }
}
