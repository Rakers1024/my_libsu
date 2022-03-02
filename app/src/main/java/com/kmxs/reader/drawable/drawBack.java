package com.kmxs.reader.drawable;

import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;

import androidx.annotation.NonNull;

public class drawBack extends Drawable {
    private Paint mPaint;
    private int[] mColor;

    public drawBack() {
        mPaint = new Paint();;
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        try {

            Rect quYu = getBounds();

            int width = quYu.right;

            int height = quYu.bottom;

            canvas.drawColor(0xFF4195F8);

            mPaint.setColor(0xFF5CA8FE);

            mPaint.setAntiAlias(true);

            mPaint.setStyle(Paint.Style.FILL);

            mPaint.setShadowLayer(40,0,0,0xFF4195f0);

            Path path3 = new Path();

            path3.moveTo(width*.5f,0);

            path3.quadTo(width*.68f,width*.28f,width*.8f,width*.2f);

            path3.quadTo(width*.92f,width*.08f,width*.98f,width*.35f);

            path3.quadTo(width*.986f,width*.37f,width,width*.36f);

            path3.lineTo(width,height);

            path3.lineTo(0,height);

            path3.lineTo(0,0);

            canvas.drawPath(path3,mPaint);

            canvas.drawRoundRect(new RectF(-width*.35f,-width*.35f,width*.35f,width*.35f),width*.35f,width*.35f,mPaint);

            Path path = new Path();

            path.moveTo(0,height-width*.3f);

            path.quadTo(width*.25f,height-width*.4f,width*.3f,height-width*.27f);

            path.quadTo(width*.35f,height-width*.12f,width*.5f,height-width*.13f);

            path.quadTo(width*.64f,height-width*.16f,width*.65f,height);

            path.lineTo(0,height);

            //c.drawPath(path,p);

            Path path2 = new Path();

            path2.moveTo(width*.65f,0);

            path2.quadTo(width*.72f,width*.12f,width*.79f,width*.05f);

            path2.quadTo(width*.85f, 0,width*.89f,width*.12f);

            path2.quadTo(width*.93f,width*.23f,width,width*.2f);

            path2.lineTo(width,0);

            canvas.drawPath(path2,mPaint);

            canvas.drawRoundRect(new RectF(width*.6f,width*.05f,width*.62f,width*.07f),width*.01f,width*.01f,mPaint);

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

