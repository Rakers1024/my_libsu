package com.kmxs.reader.drawable;

import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;

public class drawConcave extends Drawable {
    private Paint mPaint;
    private int mColor;

    public drawConcave(int col) {
        mPaint = new Paint();
        mColor = col;
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        try {


            Rect quYu = getBounds();

            int width = quYu.right;

            int height = quYu.bottom;

            mPaint.setColor(mColor);
            mPaint.setAntiAlias(true);
            mPaint.setStyle(Paint.Style.FILL);

            Path path = new Path();
            path.moveTo(0, height / 2);
            path.quadTo(width / 2, height, width, height / 2);
            path.lineTo(width, height);
            path.lineTo(0, height);
            path.lineTo(0, height / 2);
            path.close();
            canvas.drawPath(path, mPaint);
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
