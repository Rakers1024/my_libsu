package me.simple.lib.injection;

import android.app.AppOpsManager;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;

import androidx.annotation.RequiresApi;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Collection;

import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ShellUtils;

public class RootUtil {

    final private static String TAG = "inject-RootUtil";

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static void allowAllPermissions(Context context, String packageName)
    {
        try
        {
            PackageInfo pi = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_PERMISSIONS);
            for (String permissionName : pi.requestedPermissions)
            {
                try
                {
                    runShell("pm grant " + packageName + " " + permissionName, true);
                    String opstr = AppOpsManager.permissionToOp(permissionName);
                    runShell("appops set --uid " + packageName + " " + opstr + " allow", true);
                    Log.d(TAG, "allowAllPermissions: " + permissionName + ", " + opstr);
                }
                catch (Throwable th)
                {

                }
            }
        }
        catch (Throwable th)
        {
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static void allowAllPermissions(Context mContext) {
        try {
            String[] PERMISSIONS_LIST = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), PackageManager.GET_PERMISSIONS).requestedPermissions;
            for (String s : PERMISSIONS_LIST) {
                LogUtils.D(TAG, "appops set "+AppOpsManager.permissionToOp(s));
                ShellUtils.execCmd("appops set " + mContext.getPackageName() + " " + AppOpsManager.permissionToOp(s) + " allow", true);
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }

    //Result is have "Error"
    public static String runShell(String command, boolean isRoot)
    {
        try
        {
            Process process = Runtime.getRuntime().exec(isRoot ? "su -mm" : "sh");
            InputStream ins = process.getInputStream();
            InputStream es = process.getErrorStream();
            OutputStream ous = process.getOutputStream();
            ous.write("\n".getBytes());
            ous.flush();
            ous.write(command.getBytes());
            ous.flush();
            ous.write("\n".getBytes());
            ous.flush();
            ous.write("exit".getBytes());
            ous.flush();
            ous.write("\n".getBytes());
            ous.flush();
            byte[] result = readInputStream(ins, false);
            byte[] error = readInputStream(es, false);
            process.waitFor();
            ins.close();
            es.close();
            ous.close();
            if (new String(error).trim().isEmpty())
            {
                Log.d(TAG, new String(result));
                return new String(result);
            }
            else
            {
                String msg = "Shell Result : \n" + new String(result) + "\n" + "Shell Error : \n" + new String(error) + "\n";
                Log.d(TAG, msg);
                return msg;
            }
        }
        catch (Throwable th)
        {
            LogUtils.D(TAG, "Application Error : \n" + Log.getStackTraceString(th));
            return "Application Error : \n" + Log.getStackTraceString(th);
        }
    }

    /**
     * 打开程序
     * @param isMemoryRecord
     */
    public static void startPackage(boolean isMemoryRecord,String GAME_PACKAGE_NAME) {

        /**
         *  结束进程
         *  adb shell " kill -9 <进程号> "
         *  adb shell " am force-stop " <包名> "
         */
        //RootUtil.runShell("am force-stop " + GAME_PACKAGE_NAME, true);
        //Intent launchIntentForPackage = getPackageManager().getLaunchIntentForPackage(GAME_PACKAGE_NAME);
        //launchIntentForPackage.putExtra("MemoryRecord", "Test");
        //startActivity(launchIntentForPackage);

      //  RootUtil.runShell("/system/bin/am force-stop " + "com.tencent.tmgp.pubgmhd", true);
     //   RootUtil.runShell("/system/bin/am force-stop " + "com.tencent.ig", true);
        RootUtil.runShell("/system/bin/am force-stop " + GAME_PACKAGE_NAME, true);
        //RootUtil.runShell("/system/bin/am force-stop " + GAME_PACKAGE_NAME, true);
        //RootUtil.runShell("/system/bin/am start -n " + GAMEPACKAGE + "/com.epicgames.ue4.SplashActivity" + (isMemoryRecord ? " --es MemoryRecord \"Test\"" : ""), true);
        RootUtil.runShell("/system/bin/am start -a android.intent.action.MAIN -c android.intent.category.LAUNCHER -n " + GAME_PACKAGE_NAME + "/com.epicgames.ue4.SplashActivity --es " + (isMemoryRecord ? "MemoryRecord \"Test\"" : "TestKey \"TestValue\""), true);

    }


    public static byte[] readInputStream(InputStream ins, boolean close)
    {
        try
        {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            int i = -1;
            byte[] buf = new byte[1024];
            while ((i = ins.read(buf)) != -1)
            {
                bos.write(buf, 0, i);
            }
            if (close)
            {
                ins.close();
                bos.close();
            }
            return bos.toByteArray();
        }
        catch (Throwable th)
        {
            return Log.getStackTraceString(th).getBytes();
        }
    }
    public static void loadSoAsRoot(String soPath, String arg1, String arg2) {
        String command = "su -c app_process32 -Djava.class.path=" + ContextUtil.getApplicationContext().getPackageCodePath() + " /system/bin --nice-name=root_java " + RootUtil.class.getName() + " " + soPath + " " + arg1 + " " + arg2;
        try {
            Runtime.getRuntime().exec(command);
        } catch (IOException e) {
            e.printStackTrace();
        }
        Log.d(TAG, "launched");
    }

    public static void main(String[] args) {
        Log.d(TAG, "main: root-java-started");
        try {
            Runtime.getRuntime().load(args[0]);
            callNative(args[1], args[2]);
        }catch (Throwable th){
            Log.e(TAG, "main: ", th);
        }
        Log.d(TAG, "main: root-java-finished");
    }

    private native static void callNative(String arg1, String arg2);

}



