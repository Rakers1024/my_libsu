package me.simple.lib.inject;

import android.content.pm.ApplicationInfo;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ReflectUtils;

public class NotifyHook {

    private static final String FILTER_PACKAGE_NAME = "com.google.";
    private static final String TAG = "NotifyHook";

    public static void doHook(ClassLoader classLoader) {
        String className = "com.android.server.am.ServiceRecord";
        String methodName = "postNotification";
        try {
            Method method = ReflectUtils.findMethod(classLoader.loadClass(className), methodName);
            XposedBridge.hookMethod(method, new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                    Object o = param.thisObject;
                    Field appInfoFiled = ReflectUtils.findFiled(o.getClass(), "appInfo");
                    Field foregroudFiled = ReflectUtils.findFiled(o.getClass(), "foregroundId");
                    if (null != appInfoFiled) {
                        appInfoFiled.setAccessible(true);
                        foregroudFiled.setAccessible(true);
                        ApplicationInfo applicationInfo = (ApplicationInfo) appInfoFiled.get(o);
                        if (applicationInfo.packageName.contains(FILTER_PACKAGE_NAME) || applicationInfo.packageName.contains("com.kmxs."))
                        {
                           // LogUtils.D("GameInject","修改foregroudId");
                            LogUtils.D("GameInject", "隐藏下拉通知" );
                            XposedBridge.log("隐藏下拉通知");
                            //修改foregroudId = 0;
                            foregroudFiled.set(o, 0);
                        }
                    }
                }
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                }
            });
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}

