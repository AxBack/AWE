package com.wallpaper.axb.connections;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.service.wallpaper.WallpaperService;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ConnectionsService extends WallpaperService {

    private ConnectionsEngine mEngine;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    public Engine onCreateEngine() {
        mEngine = new ConnectionsEngine();
        return mEngine;
    }

    private class ConnectionsEngine extends WallpaperService.Engine {

        private ConnectionsSurfaceView mSurfaceView;
        private ConnectionsRenderer mRenderer;

        private float mTouchX = 0;
        private float mTouchY = 0;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);

            mSurfaceView = new ConnectionsSurfaceView(ConnectionsService.this);
            mSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 1);
            mSurfaceView.setEGLContextClientVersion(3);
            mSurfaceView.setPreserveEGLContextOnPause(true);
            mSurfaceView.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR);

            mRenderer = new ConnectionsRenderer(getAssets(), mSurfaceView.getHolder().getSurfaceFrame());
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

        private class ConnectionsSurfaceView extends GLSurfaceView {

            public ConnectionsSurfaceView(Context context) {
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
