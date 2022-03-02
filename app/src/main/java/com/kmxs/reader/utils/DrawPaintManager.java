package com.kmxs.reader.utils;

import com.kmxs.reader.core.Config;
import com.kmxs.reader.core.Config;

import com.kmxs.reader.App;

import me.simple.lib.utils.LogUtils;

public class DrawPaintManager {

    private static volatile DrawPaintManager instance = null;

    private Config drawPaintConfig;

    private DrawPaintManager() {
        getPaintConfig();
    }
    public static DrawPaintManager getInstance() {
        if (instance == null) {
            synchronized (DrawPaintManager.class) {
                if (instance == null) {
                    instance = new DrawPaintManager();
                }
            }
        }
        return instance;
    }

    private void getPaintConfig() {
        drawPaintConfig = new Config();
        String[] temp = App.ReadConfig("Offset", "").split("x");
        if (temp.length == 2) {
            drawPaintConfig.setmLeftOffset(BearHelper.toInt(temp[1]));
            drawPaintConfig.setmUpOffset(BearHelper.toInt(temp[0]));
        } else {
            drawPaintConfig.setmLeftOffset(0);
            drawPaintConfig.setmUpOffset(0);
        }
    }

    public float getProportion() {
        return drawPaintConfig.mProportion;
    }

    public void setProportion(String value) {
        String[] temp = value.split("x");
        drawPaintConfig.setmProportion(BearHelper.toNumber(temp[2]) / 100);

        LogUtils.D("地图配置", drawPaintConfig.mProportion + "");
    }

    public float[] getMap() {
        return drawPaintConfig.mMap;
    }

    public void setMap(String value) {
        String[] temp = value.split("x");
        drawPaintConfig.mMap[0] = BearHelper.toNumber(temp[0]);
        drawPaintConfig.mMap[1] = BearHelper.toNumber(temp[1]);
        LogUtils.D("地图配置", drawPaintConfig.mMap[0] + "x" + drawPaintConfig.mMap[1]);
    }

    public int getUpOffset() {
        return drawPaintConfig.mUpOffset;
    }

    public void setUpOffset(int value) {
        drawPaintConfig.mUpOffset = value;
        App.SaveConfig("Offset", drawPaintConfig.mUpOffset + "x" + drawPaintConfig.mLeftOffset);
    }
    public int getLeftOffset() {
        return drawPaintConfig.mLeftOffset;
    }

    public void setLeftOffset(int value) {
        drawPaintConfig.mLeftOffset = value;
        App.SaveConfig("Offset", drawPaintConfig.mUpOffset + "x" + drawPaintConfig.mLeftOffset);
    }
}
