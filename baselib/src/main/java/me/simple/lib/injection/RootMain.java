package me.simple.lib.injection;

import android.content.Context;
import android.graphics.Point;
import android.os.Build;
import android.util.Log;
import android.view.WindowManager;

import androidx.annotation.RequiresApi;


import java.io.IOException;

import me.simple.lib.utils.LogUtils;

public class RootMain {
    private static final String TAG = "GameInject";

    private static native void Test();

    public static void launch() {
        String command = "su -c app_process32 -Djava.class.path=" + ContextUtil.getApplicationContext().getPackageCodePath() + " /system/bin --nice-name=touch_root_java " + RootMain.class.getName();
        try {
            Runtime.getRuntime().exec(command);
        } catch (IOException e) {
            e.printStackTrace();
        }
        LogUtils.D(TAG, "launched:"+ContextUtil.getApplicationContext().getPackageCodePath());
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static void main(String[] args) {
        //FileUtils.findLibrary(Context context, String libName)
        Log.d(TAG, "main: root-java-started:"+ContextUtil.getApplicationContext().getApplicationInfo());
        String libPath =ContextUtil.getApplicationContext().getApplicationInfo().nativeLibraryDir + "/libapp_test.so";
      //  dataDir = getPackageManager().getPackageInfo(getPackageName(), 0).applicationInfo.dataDir;
        try {
            Runtime.getRuntime().load(libPath);
            Test();
            Point size = new Point();
            ContextUtil.getSystemContext().getSystemService(WindowManager.class).getDefaultDisplay().getRealSize(size);
  //          doAimbot(Params.GAMEPACKAGE, ContextUtil.getGameContext(ContextUtil.getSystemContext()).getApplicationInfo().nativeLibraryDir + "/libmemoryrecord.py.so", "libmemoryrecord.py.so", size.x, size.y);
            LogUtils.D(TAG, "root-java-finished");
        }catch (Throwable th){
            Log.e(TAG, "main: ", th);
        }
    }
}
