package com.kmxs.reader.utils;

import android.graphics.Color;
import android.graphics.Paint;


public class ColorUtil {
    public static boolean onlyGreen = false;

    private static final String[] colors = {
            "#00CC00",
            "#00ffff",
            "#0000ff",
            "#ffc0cb",
            "#FF4500",//6a5acd
            "#ffa500",
            "#ff00ff",
            "#ff0000"
    };

    public static int setAlpha(int alpha, int color) {
        return Color.argb(alpha, Color.red(color), Color.green(color), Color.blue(color));
    }

    public static int getColorByID(int id) {
        if (onlyGreen) {
            return Color.GREEN;
        }
        int index = id % colors.length;
        if (index < 0){
            index = index * -1;
        }
        return Color.parseColor(colors[index]);
    }

    public static Paint createPaint(Paint.Style style, int color, int width, int textSize, Paint.Align textAlign) {
        Paint newPaint = new Paint();
        newPaint.setAlpha(0);//0-255透明度
        newPaint.setColor(color);//颜色
        newPaint.setStyle(style);//Style 边/实心
        newPaint.setStrokeCap(Paint.Cap.ROUND);//Cap 线帽 B无 R圆 S方 线帽会多出来一节
        newPaint.setStrokeJoin(Paint.Join.ROUND);//转折 M尖 B平 R圆
        newPaint.setStrokeWidth(width);//线粗
        newPaint.setAntiAlias(true);//抗锯齿
        newPaint.setDither(true);//鲜艳

        newPaint.setTextSize(textSize);//字号
        newPaint.setTextAlign(textAlign);//对齐方式
       // newPaint.setTypeface(Params.font);
        return newPaint;
    }

    public static int getTextBaseLine(Paint paint) {
        Paint.FontMetricsInt fontMetrics = paint.getFontMetricsInt();
        return (fontMetrics.bottom - fontMetrics.top) / 2 - fontMetrics.bottom;
    }
}

