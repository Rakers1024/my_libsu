package me.simple.lib;

import android.annotation.SuppressLint;
import android.app.Application;
import android.content.Context;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import androidx.annotation.RequiresApi;

import es.dmoral.toasty.Toasty;
import me.simple.lib.utils.ReflectionApiCheck;

public class BaseContentApp extends Application {

    @SuppressLint("StaticFieldLeak")
    public static Context mContext;
    private static int ScreenHeight,RealHeight,ScreenWidth,RealWidth,StatusBarHeight;

    private static String mFilePath;

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)

    @Override
    public void onCreate() {
        super.onCreate();
        ReflectionApiCheck.exemptAll();
        mContext = this;
        Toasty.Config.getInstance().setTextSize(12).apply();
        getSystemInfo();
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    private void getSystemInfo() {
        mFilePath = getFilesDir().toString();
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            StatusBarHeight = getResources().getDimensionPixelSize(resourceId);
        } else {
            StatusBarHeight = 0;
        }

        DisplayMetrics dm = getResources().getDisplayMetrics();
        if (dm.heightPixels > dm.widthPixels) {
            ScreenHeight = dm.heightPixels;
            ScreenWidth = dm.widthPixels;
        } else {
            ScreenHeight = dm.widthPixels;
            ScreenWidth = dm.heightPixels;
        }

        WindowManager wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics metrics = new DisplayMetrics();
        wm.getDefaultDisplay().getRealMetrics(metrics);
        if (metrics.heightPixels > metrics.widthPixels) {
            RealHeight = metrics.heightPixels;
            RealWidth = metrics.widthPixels;
        } else {
            RealHeight = metrics.widthPixels;
            RealWidth = metrics.heightPixels;
        }
    }

    public static String getFilePath() {
        return mFilePath;
    }

    public static int getScreenHeight() {
        return ScreenHeight;
    }

    public static int getScreenWidth() {
        return ScreenWidth;
    }

    public static int getRealHeight() {
        return RealHeight;
    }

    public static int getRealWidth() {
        return RealWidth;
    }

    public static int getStatusBarHeight() {
        return StatusBarHeight;
    }
}
