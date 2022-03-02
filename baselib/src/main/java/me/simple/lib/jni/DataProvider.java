package me.simple.lib.jni;


import android.content.res.AssetManager;
import android.view.MotionEvent;
import android.view.Surface;

import me.simple.lib.data.DrawData;
import me.simple.lib.services.RealDataProvider;

public class DataProvider {

    public static boolean isInited = false;
    public static boolean isInited2 = false;
    public static synchronized void init(String filePath,String filePath2) {
        if (!isInited)
        {
            isInited = true;
            System.load(filePath);
           // System.load(filePath2);
        }
    }
    static {
        if (!isInited2) {
            isInited2 = true;
           // System.loadLibrary("system_app");
        }
    }

    public static native void setSurface(Surface surface);

    public static native void navateInputEvent(MotionEvent motionEvent);

    public static native float getMenuW();

    public static native float getMenuH();

    public static native float getMenuX();

    public static native float getMenuY();

    public static native void setScreenSize(int x,int y);

    public static native void InitAssets(AssetManager assets);

    public static native void setAimData(int aimbotmode,boolean isAimbot ,int aimingState,int priority,boolean pour,float aimingRange, int gun,int prognosis,int touchSpeed,int touchSpeed1,int touchX,int touchY,int PressureGunData,int aimLock,boolean isAimAi,int ScreenWidth, int ScreenHeight,boolean isPeople ,
                                         boolean isItems,
                                         boolean isVehicle,
                                         boolean isMissiles,boolean isbunkers,boolean isFlipScreen ,boolean isBone,boolean isVisible,int aimMode,float memoryPressure,float mempryAimSpeed,float Anticipation,int testValue);

    //  public static native DrawData ReadData(RealDataProvider realDataProvider);
    public static native DrawData ReadData();

    public static native int start(String token,String charge,String user, int h, int w,boolean isBlackShark);

    public static native void stop();

    public static native void setPackageName(String name);

    public static native void setToken(int t);

    public native static void doDraw(Class drawClass);

    public static native void getRadarData(float x,float y,float p);

    public static native void imguiInit();

    public static native void imguiResize(int width, int height);

    public static native void imguiRenderer();

    public static native boolean switchWindowStatus();
}
