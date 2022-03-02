package com.kmxs.reader.core.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.util.AttributeSet;
import android.view.View;

public class MapView extends View  {

    private final Paint paint = new Paint();
    private final Paint paint2 = new Paint();

    public MapView(Context context) {
        super(context);
        init();
    }

    public MapView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public MapView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }


    private void init() {
        paint.setStrokeWidth(7);
        paint.setStyle(Paint.Style.STROKE);
        paint.setColor(Color.parseColor("#000000"));

        paint2.setStrokeWidth(3);
        paint2.setColor(Color.parseColor("#ffc0cb"));
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(0, PorterDuff.Mode.CLEAR);
        canvas.drawLine(0, 0, getWidth(), getHeight(), paint2);
        canvas.drawLine(getWidth(), 0, 0, getHeight(), paint2);
        canvas.drawRect(0, 0, getWidth(), getHeight(), paint);
        canvas.drawRect(getWidth() / 2 - 6, getHeight() / 2 - 6, getWidth() / 2 + 6, getHeight() / 2 + 6, paint2);
    }



}


