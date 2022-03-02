package com.kmxs.reader.drawable;

import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;

public class drawButton extends Drawable {
    private Paint mPaint;
    private int mColor;

    public drawButton(int col) {
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

            mPaint.setShadowLayer(0,0,0,0x99ffffff);

            canvas.drawRoundRect(new RectF(0,0,width,height),height/2,height/2,mPaint);
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

