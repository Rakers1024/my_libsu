package me.simple.lib.utils;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.reflect.Field;
import java.util.Arrays;


public class CrashHandlerUtil implements Thread.UncaughtExceptionHandler {

    //CrashHandler实例
    @SuppressLint("StaticFieldLeak")
    private static volatile CrashHandlerUtil instance = null;
    //程序的Context对象
    private Context mContext;

    private Class mActivity;

    //来存储设备信息和异常信息用
    public static String logstr;

    private Thread.UncaughtExceptionHandler mDefaultHandler;

    public static CrashHandlerUtil getInstance() {
        if (instance == null) {
            synchronized (CrashHandlerUtil.class) {
                if (instance == null) {
                    instance = new CrashHandlerUtil();
                }
            }
        }
        return instance;
    }


    public void init(Context context, Class a) {
        mContext = context;
        mActivity = a;
        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(this);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public void uncaughtException(@NonNull Thread thread, @NonNull Throwable ex) {
        if (!handleException(ex) && mDefaultHandler != null) {
            mDefaultHandler.uncaughtException(thread, ex);
        } else {
            ActivityManger.finishAll();
            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(1);
        }

    }

    private static String getAndroidName() {
        String str = "";
        for (Field field : Build.VERSION_CODES.class.getFields()) {
            try {
                if (field.getInt(new Object()) == Build.VERSION.SDK_INT) {
                    str = field.getName();
                }
            } catch (IllegalArgumentException | IllegalAccessException e) {
                e.printStackTrace();
            }
        }
        return str;
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @SuppressLint("WrongConstant")
    private boolean handleException(Throwable ex) {
        if (ex == null) {
            return false;
        }
        try {
            @SuppressLint("WrongConstant")
            PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 1);
            StringBuilder a = new StringBuilder("应用版本: ");
            a.append(packageInfo.versionName).append("\n");
            a.append("Android版本: Android ");
            a.append(getAndroidName());
            a.append(" ").append(Build.VERSION.RELEASE).append(" ");
            a.append("(API ");
            a.append(Build.VERSION.SDK_INT).append(")\n");
            a.append("手机厂家: ");
            a.append(Build.MANUFACTURER).append("\n");
            a.append("手机型号: ");
            a.append(Build.MODEL).append("\n");
            a.append("CPU指令集: ");
            a.append(Arrays.toString(Build.SUPPORTED_ABIS));
            a.append("\n");
            a.append("调试内容:\n");
            String result = getStackTrace(ex);
            a.append(result);
            logstr = a.toString();
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        mContext.startActivity(new Intent(mContext, mActivity).addFlags(0x10000000).putExtra("CashStr", logstr));
        return true;
    }

    private String getStackTrace(Throwable th) {
        StringWriter stringWriter = new StringWriter();
        th.printStackTrace(new PrintWriter(stringWriter));
        return stringWriter.toString();
    }

}

