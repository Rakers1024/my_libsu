package com.kmxs.reader.utils;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.text.TextUtils;
import android.view.WindowManager;

import androidx.annotation.RequiresApi;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

import me.simple.lib.utils.LogUtils;

public class RecorderFakeUtils {
    //华为
    public static boolean isEmui() {
        LogUtils.D("GameInject","华为");

        return !TextUtils.isEmpty(getProp("ro.build.version.emui"));
    }

    //小米
    public static boolean isMiui() {
        LogUtils.D("GameInject","小米");
        return !TextUtils.isEmpty(getProp("ro.miui.ui.version.name")) || !TextUtils.isEmpty(getProp("ro.blackshark.rom"));
    }

    //vivo
    public static boolean isVivo() {
        LogUtils.D("GameInject","vivo");

        return !TextUtils.isEmpty(getProp("ro.vivo.os.version"));
    }

    //oppo
    public static boolean isOppo() {
        LogUtils.D("GameInject","Oppo");

        return !TextUtils.isEmpty(getProp("ro.build.version.opporom"));
    }

    //魅族
    public static boolean isFlyme() {
        LogUtils.D("GameInject","魅族");

        return Build.DISPLAY.toUpperCase().contains("FLYME");
    }

    //努比亚
    public static boolean isNubia() {
        LogUtils.D("GameInject","努比亚");

        return !TextUtils.isEmpty(getProp("ro.build.nubia.rom.name"));
    }

    //锤子坚果
    public static boolean isSmartisan() {
        return !TextUtils.isEmpty(getProp("ro.smartisan.version"));
    }


    //三星
    public static boolean isOneUi() {
        return !TextUtils.isEmpty(getProp("ro.config.knox"));
    }

    //一加
    public static boolean isHydrogen() {
        LogUtils.D("GameInject","一加");
        return !TextUtils.isEmpty(getProp("ro.build.ota.versionname")) && getProp("ro.build.ota.versionname").toLowerCase().contains("hydrogen");
    }

    //华硕
    public static boolean isAsus() {
        return !TextUtils.isEmpty(getProp("ro.asus.rog"));
    }

    public static void setMiuiParams(WindowManager.LayoutParams params) {
        params.flags |= 4096;
    }

    public static void setNubiaParams(WindowManager.LayoutParams params) {
        params.flags |= 8;
        params.format = -3;
    }



    public static void setAsusParams(WindowManager.LayoutParams params) {
        params.memoryType |= 0x10000000;
    }


    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    public static void setWindowLayoutParams(Context context, WindowManager.LayoutParams layoutParams) {
        try {
            if (isMiui()) {
                setMiuiParams(layoutParams);
            } else if (isFlyme()) {
                if (!setMeizuParams(layoutParams)) {

                    setMeizuParamsNew(context, layoutParams);
                }
            } else if (isOneUi()) {
                setOneUiParams(layoutParams);
            } else if (isAsus()) {
                setAsusParams(layoutParams);
            } else if (isHydrogen()) {
                setAsusParams(layoutParams);
            } else if (isNubia()) {
                setNubiaParams(layoutParams);
            }
            layoutParams.setTitle(getFakeRecordWindowTitle());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static boolean setMeizuParams(WindowManager.LayoutParams params) {
        try {
            Class mzlp = Class.forName("android.view.MeizuLayoutParams");
            Field flags = mzlp.getDeclaredField("flags");
            flags.setAccessible(true);
            Object obj = mzlp.newInstance();
            flags.setInt(obj, 8192);
            params.getClass().getField("meizuParams").set(params, obj);
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static void setMeizuParamsNew(Context context, WindowManager.LayoutParams params) {
        if (context.checkSelfPermission("flyme.permission.SKIP_CAPTURE") != PackageManager.PERMISSION_GRANTED) {
            try {
                Field 魅族悬浮窗新 = params.getClass().getDeclaredField("meizuFlags");
                魅族悬浮窗新.setAccessible(true);
                魅族悬浮窗新.setInt(params, 1024);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }


    public static void setHydrogenParams(WindowManager.LayoutParams params) {
        try {
            Field gone = params.getClass().getDeclaredField("PRIVATE_FLAG_IS_ROUNDED_CORNERS_OVERLAY");
            gone.setAccessible(true);
            Object obj = gone.get(gone.getClass());
            if (obj != null) {
                Field flags = params.getClass().getDeclaredField("privateFlags");
                flags.setAccessible(true);
                flags.set(params, Integer.valueOf(((Integer) obj).intValue()));
            }
        } catch (NoSuchFieldException | IllegalAccessException e) {
            e.printStackTrace();
        }
    }

    public static void setOneUiParams(WindowManager.LayoutParams params) {
        try {
            Method flags = params.getClass().getMethod("semAddExtensionFlags", new Class[]{Integer.TYPE});
            Method flags1 = params.getClass().getMethod("semAddPrivateFlags", new Class[]{Integer.TYPE});
            flags.invoke(params, new Object[]{-2147352576});
            flags1.invoke(params, new Object[]{Integer.valueOf(params.flags)});
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    /**
     * 总结下可以操作的是华为，魅族，oppo
     *
     * @return
     */
    public static String getFakeRecordWindowTitle() {
        if (!TextUtils.isEmpty(getProp("ro.miui.ui.version.name"))) {//MIUI
            return "com.miui.screenrecorder";
        } else if (!TextUtils.isEmpty(getProp("ro.blackshark.rom"))) {//黑鲨UI
            return "com.blackshark.screenrecorder";
        } else if (!TextUtils.isEmpty(getProp("ro.build.version.emui"))) {//华为UI
            return " ScreenRecoderTimer";
        } else if (!TextUtils.isEmpty(getProp("ro.vivo.os.version"))) {//vivo
            return "screen_record_menu";
        } else if (!TextUtils.isEmpty(getProp("ro.build.version.opporom"))) {//oppo
            return "com.coloros.screenrecorder.FloatView";
        } else if (!TextUtils.isEmpty(getProp("ro.smartisan.version"))) {//锤子坚果
            return "";
        } else if (!TextUtils.isEmpty(getProp("ro.build.nubia.rom.name"))) {//努比亚傻逼东西,不能过直播
            return "NubiaRecordFloatingPanel";//"NubiaScreenDecorOverlay";
        } else if (!TextUtils.isEmpty(getProp("ro.asus.rog"))) {//华硕
            return "com.asus.force.layer.transparent.SR.floatingpanel";
        } else if (!TextUtils.isEmpty(getProp("ro.build.ota.versionname")) && getProp("ro.build.ota.versionname").toLowerCase().contains("hydrogen")) {//一加
            return "op_screenrecord";
        } else if (!TextUtils.isEmpty(getProp("ro.config.knox"))) {//三星One UI
            return "Screen_Recording_PIP_Preview";
        } else if (Build.DISPLAY.toUpperCase().contains("FLYME")) {//魅族
            return "SysScreenRecorder";
        }
        return "";
    }

    public static String getProp(String name) {
        String line = null;
        BufferedReader input = null;
        try {
            Process p = Runtime.getRuntime().exec("getprop " + name);
            input = new BufferedReader(new InputStreamReader(p.getInputStream()), 1024);
            line = input.readLine();
            input.close();
        } catch (IOException ex) {
            return null;
        } finally {
            if (input != null) {
                try {
                    input.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return line;
    }

}