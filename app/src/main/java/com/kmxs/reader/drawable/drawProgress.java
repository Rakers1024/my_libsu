package com.kmxs.reader.drawable;

import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;

public class drawProgress extends Drawable {
    private Paint mPaint;
    private int[] mArry;
    private int mValue;

    public drawProgress(int a, int[] b) {
        mPaint = new Paint();
        mValue = a;
        mArry = b;

    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        try {

            Rect quYu = getBounds();

            int width = quYu.right;

            int height = quYu.bottom;

            mPaint.setAntiAlias(true);
            mPaint.setStrokeWidth(height);
            mPaint.setStyle(Paint.Style.STROKE);
            mPaint.setStrokeCap(Paint.Cap.ROUND);
            mPaint.setColor(mArry[0]);
            canvas.drawLine(0, height / 2, width, height / 2, mPaint);
            mPaint.setColor(mArry[1]);
            canvas.drawLine(0, height / 2, width * mValue / mArry[2], height / 2, mPaint);
            invalidateSelf();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void setAlpha(int p1) {
        mPaint.setAlpha(p1);
        // TODO: Implement this method
    }

    @Override
    public void setColorFilter(ColorFilter p1) {
        mPaint.setColorFilter(p1);
        // TODO: Implement this method
    }

    @Override
    public int getOpacity() {
        // TODO: Implement this method
        return PixelFormat.UNKNOWN;
    }

    public Paint getPaint() {
        return mPaint;
    }
}
