package me.simple.lib.inject;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.text.TextUtils;
import android.view.WindowManager;

import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.RecorderFakeUtils;
import me.simple.lib.utils.ReflectUtils;

public class WindowHook {
    public static boolean isSystemApplication(Context context, String packageName) {

        if (context == null) {
            return false;
        }
        PackageManager packageManager = context.getPackageManager();
        if (packageManager == null || packageName == null || packageName.length() == 0) {
            return false;
        }

        try {
            ApplicationInfo app = packageManager.getApplicationInfo(packageName, 0);
            return (app != null && (app.flags & ApplicationInfo.FLAG_SYSTEM) > 0);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return false;
    }

    public static void doHook(ClassLoader mClassLoader) {
        try {
            Class<?> mContextImpl = XposedHelpers.findClass("com.android.server.wm.WindowManagerService", mClassLoader);
            XposedBridge.hookAllMethods(mContextImpl, "addWindow", new XC_MethodHook() {
                protected void beforeHookedMethod(MethodHookParam param) {
                    WindowManager.LayoutParams params = (WindowManager.LayoutParams) param.args[3];
                    Context context = (Context) ReflectUtils.rejectFind(param.thisObject, "android.content.Context", "Context");
                    if (context == null) return;
                    PackageManager packageManager = context.getPackageManager();
                    if (packageManager == null) return;
                    String caller = packageManager.getNameForUid(Binder.getCallingUid());
                    if (caller == null) return;
                    if (isSystemApplication(context, caller)) return;
                    if (TextUtils.isEmpty(caller)) return;

                    if (params.type == WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY || params.type == WindowManager.LayoutParams.TYPE_SYSTEM_ALERT) {
                        RecorderFakeUtils.setFakeRecorderWindowLayoutParams(context, params);
                        LogUtils.D("GameInject", "隐藏窗口" + caller);
                        XposedBridge.log("隐藏窗口:" + caller);
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
