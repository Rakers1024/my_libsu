package me.simple.lib.services;

import android.os.RemoteException;
import android.view.MotionEvent;
import android.view.Surface;

import me.simple.lib.IIDataProvider;
import me.simple.lib.data.DrawData;


import me.simple.lib.jni.DataProvider;
import me.simple.lib.utils.LogUtils;

public class RealDataProvider extends IIDataProvider.Stub {

    public RealDataProvider() {

    }

    @Override
    public int start(String token, String charge, String user, int ScreenWidth, int ScreenHeight, boolean isBlackShark) {
        LogUtils.D("DataProviderService", "start");
        return DataProvider.start(token, charge, user, ScreenWidth, ScreenHeight, isBlackShark);
    }

    @Override
    public void setAimData(int aimbotmode, boolean isAimbot, int aimingState, int priority, boolean pour, float aimingRange,
                           int gun, int prognosis, int touchSpeed, int touchSpeed1, int touchX, int touchY, int PressureGunData, int aimLock, boolean isAimAi, int ScreenWidth, int ScreenHeight, boolean isPeople, boolean isItems,
                           boolean isVehicle,
                           boolean isMissiles, boolean isbunkers, boolean isFlipScreen, boolean isBone, boolean isVisible, int aimMode, float memoryPressure, float mempryAimSpeed, float Anticipation, int testValue) {
        LogUtils.D("setAimData", "start");
        DataProvider.setAimData(aimbotmode, isAimbot, aimingState, priority, pour, aimingRange, gun, prognosis, touchSpeed, touchSpeed1, touchX, touchY, PressureGunData, aimLock, isAimAi, ScreenWidth, ScreenHeight, isPeople, isItems, isVehicle, isMissiles, isbunkers, isFlipScreen, isBone, isVisible, aimMode, memoryPressure, mempryAimSpeed, Anticipation, testValue);
    }


    @Override
    public void stop() {
        LogUtils.D("DataProviderService", "stop");
        DataProvider.stop();
    }


    @Override
    public DrawData readData() {
        try {
            return DataProvider.ReadData();
        } catch (Exception e) {
            return null;
        }
    }


    @Override
    public void setSurface(Surface surface) throws RemoteException {
        try {
            DataProvider.setSurface(surface);
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    @Override
    public void navateInputEvent(MotionEvent motionEvent) throws RemoteException {
        try {
            DataProvider.navateInputEvent(motionEvent);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public float getMenuW() throws RemoteException {
        return DataProvider.getMenuW();
    }

    @Override
    public float getMenuH() throws RemoteException {
        return DataProvider.getMenuH();
    }

    @Override
    public float getMenuX() throws RemoteException {
        return DataProvider.getMenuX();
    }

    @Override
    public float getMenuY() throws RemoteException {
        return DataProvider.getMenuY();
    }

    @Override
    public void setScreenSize(int width, int height) throws RemoteException {
        DataProvider.setScreenSize(width, height);
    }

    @Override
    public void setPackageName(String name) throws RemoteException {
        DataProvider.setPackageName(name);
    }

    @Override
    public void getRadarData(float x, float y, float p) throws RemoteException {
        DataProvider.getRadarData(x, y, p);
    }

    @Override
    public void setToken(int t) throws RemoteException {
        DataProvider.setToken(t);
    }

    @Override
    public void imguiInit() throws RemoteException {
        DataProvider.imguiInit();
    }

    @Override
    public void imguiResize(int width, int height) throws RemoteException {
        DataProvider.imguiResize(width,height);
    }

    @Override
    public void imguiRenderer() throws RemoteException {
        DataProvider.imguiRenderer();
    }

    @Override
    public boolean switchWindowStatus() throws RemoteException {
        return DataProvider.switchWindowStatus();
    }


}
