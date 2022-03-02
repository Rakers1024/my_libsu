package com.kmxs.reader.core.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.widget.FrameLayout;

import androidx.annotation.Nullable;

import me.simple.lib.utils.LogUtils;

public class LoadView extends FrameLayout {

    private int mProgress = 0;
    private int mMaxProgress = 200;
    private int[] mColor;
    private Paint mPaint;

    public LoadView(Context context) {
        super(context);
        init();
    }

    public LoadView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public LoadView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        mPaint = new Paint();
        mColor = new int[]{0xFF4195F8, 0xFFEEEEEE};
        setWillNotDraw(false);
    }

    public void setProgress(int value) {
        mProgress = value;
        postInvalidate();
    }

    public void setColor(int[] value) {
        mColor = value;
        postInvalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        try {
            int width = getWidth();
            int height = getHeight();
            //LogUtils.D("LoadView", width + " x " + height);
            mPaint.setAntiAlias(true);

            mPaint.setStrokeWidth(width * .005f);

            mPaint.setStyle(Paint.Style.FILL);

            mPaint.setStrokeCap(Paint.Cap.ROUND);

            for (int i = 0; i <= mMaxProgress; i++) {
                mPaint.setColor(i <= mProgress ? mColor[0] : mColor[1]);
                canvas.drawLine(width / 2, height / 2 - width / 2, width / 2, height / 2 - width / 2 + 20, mPaint);
                canvas.rotate(1.8f, width / 2, height / 2);
            }
        } catch (Exception e) {
            e.printStackTrace();
            LogUtils.D("LoadView", e.getMessage());
        }
    }


}
