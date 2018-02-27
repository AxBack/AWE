package com.wallpaper.axb.view;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

import com.wallpaper.axb.engine.R;

/**
 * Created by Atte on 2018-02-27.
 */

public class SlideInView extends FrameLayout implements View.OnTouchListener {

    private int mCollapsedX;

    public SlideInView(Context context) {
        super(context);
        init(context);
    }

    public SlideInView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public SlideInView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        final View view = inflate(context, R.layout.slide_in_view, this);
        setOnTouchListener(this);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);

        // set X to collapsed mode
        final int iconWidth = findViewById(R.id.slide_in_view_icon).getMeasuredWidth();
        final int width = getMeasuredWidth();
        mCollapsedX = (iconWidth - width);
        setX(mCollapsedX);
    }


    float touchStartX;
    @Override
    public boolean onTouch(View view, MotionEvent event) {
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                touchStartX = getX() - event.getRawX();
            case MotionEvent.ACTION_MOVE:
                float newX = Math.min(0, Math.max(mCollapsedX, (event.getRawX() + touchStartX)));
                setX(newX);
                return true;
        }
        return false;
    }
}
