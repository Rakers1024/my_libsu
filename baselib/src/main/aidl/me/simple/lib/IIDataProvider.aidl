// IIDataProvider.aidl
package me.simple.lib;

// Declare any non-default types here with import statements
import me.simple.lib.data.DrawData;
import android.view.MotionEvent;
import android.view.Surface;


interface IIDataProvider {
   int start(String token,String charge,String user,int ScreenWidth, int ScreenHeight,boolean isBlackShark);
   void  setAimData(int aimbotmode,boolean isAimbot ,int aimingState,int priority,boolean pour,float aimingRange,int gun,int prognosis,int touchSpeed,int touchSpeed1,int touchX,int touchY,int PressureGunData,int aimLock,boolean isAimAi,int ScreenWidth, int ScreenHeight, boolean isPeople ,boolean isItems, boolean isVehicle,boolean isMissiles ,boolean isbunkers,boolean isFlipScreen,boolean isBone,boolean isVisible,int aimMode,float memoryPressure,float mempryAimSpeed ,float Anticipation,int testValue);
   void stop();
   DrawData readData();
   void setSurface( in Surface surface);
   void navateInputEvent( in MotionEvent motionEvent);
   float getMenuW();
   float getMenuH();
   float getMenuX();
   float getMenuY();
   void setScreenSize(int width, int height);
   void setPackageName(String name);
   void getRadarData(float x,float y,float p);
   //void InitAssets(in AssetManager assets);
   void setToken(int t);

   void imguiInit();
   void imguiResize(int width, int height);
   void imguiRenderer();
   boolean switchWindowStatus();
}