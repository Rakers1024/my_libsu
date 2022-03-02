package me.simple.lib.inject;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.text.TextUtils;

import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ReflectUtils;

public class ActivityManagerServiceHook {
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
            Class<?> mContextImpl = XposedHelpers.findClass("com.android.server.am.ActivityManagerService", mClassLoader);
            XposedBridge.hookAllMethods(mContextImpl, "checkPermission", new XC_MethodHook() {
                protected void beforeHookedMethod(MethodHookParam param) {
                    String Permission = (String) param.args[0];
                    if (Permission == null) return;
                    if (Permission.equals("android.permission.INTERNAL_SYSTEM_WINDOW") || Permission.equals("flyme.permission.SKIP_CAPTURE")) {

                        Context context = (Context) ReflectUtils.rejectFind(param.thisObject, "android.content.Context", "Context");
                        if (context == null) return;
                        PackageManager packageManager = context.getPackageManager();
                        if (packageManager == null) return;
                        String caller = packageManager.getNameForUid(Binder.getCallingUid());
                        if (caller == null) return;
                        if (isSystemApplication(context, caller)) return;

                        LogUtils.D("GameInject", "申请权限 " + Permission + " caller:" + caller);

                        if (!TextUtils.isEmpty(caller)) {
                            try {
                                ApplicationInfo appInfo = packageManager.getPackageInfo(caller, PackageManager.GET_META_DATA).applicationInfo;
                                boolean hiderecode = appInfo.metaData.getBoolean("hiderecord");
                                // if (hiderecode)
                                {
                                    LogUtils.D("GameInject", " 开启隐藏效果：" + caller);
                                    XposedBridge.log("开启隐藏效果:" + caller);
                                    param.setResult(PackageManager.PERMISSION_GRANTED);
                                }
                            } catch (PackageManager.NameNotFoundException e) {
                                e.printStackTrace();
                            }
                        }


                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
