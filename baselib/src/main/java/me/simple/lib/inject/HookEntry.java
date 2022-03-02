package me.simple.lib.inject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.Application;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageItemInfo;
import android.content.pm.PackageManager;
import android.content.pm.ProviderInfo;
import android.content.pm.ResolveInfo;
import android.content.pm.VersionedPackage;
import android.content.res.Resources;
import android.graphics.Point;
import android.hardware.input.InputManager;

import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.opengl.GLSurfaceView;
import android.os.Binder;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;
import java.util.Random;
import java.util.UUID;


import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XC_MethodReplacement;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;
import es.dmoral.toasty.Toasty;

import me.simple.lib.glview.GLView;
import me.simple.lib.injection.ContextUtil;
import me.simple.lib.injection.RootUtil;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.RecorderFakeUtils;
import me.simple.lib.utils.ReflectUtil;
import me.simple.lib.utils.ReflectUtils;
import me.simple.lib.utils.ReflectionApiCheck;

import android.content.pm.*;

import android.content.pm.PackageInfo;
import android.content.pm.PackageItemInfo;
import android.content.pm.PackageManager;

import android.content.pm.ProviderInfo;
import android.content.pm.ResolveInfo;
import android.content.pm.VersionedPackage;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static android.content.Context.WINDOW_SERVICE;
import static de.robv.android.xposed.XposedHelpers.findAndHookMethod;


public class HookEntry implements IXposedHookLoadPackage {

    public static boolean coreLoaded = false;
    public static String packageName = "com." + getRandomString(5) + "." + getRandomString(6);
    public static String settingsId = getRandomString(15);
    public static String id = getRandomString(15);


    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) {
        if (lpparam.packageName.equals("android")) {
            ReflectionApiCheck.exemptAll();
            ActivityManagerServiceHook.doHook(lpparam.classLoader);//获取特殊权限
            NotifyHook.doHook(lpparam.classLoader);//过滤通知栏
            //WindowHook.doHook(lpparam.classLoader);//隐藏窗口
        }
        String packageName = (String) ReflectUtils.rejectFind(lpparam, "java.lang.String", "packageName");
        //用来判断是否激活
       /* switch (packageName) {
            case "com.tencent.tmgp.pubgmhd":
            case "com.tencent.tmgp.projectg":
            case "com.tencent.ig":
                afterApplicationPug(lpparam.classLoader);
                doHookMediaProjection();
              //  doHookXposed();
                doHookPack(lpparam.classLoader);//隐藏应用
                break;
        }*/
        //用于激活判断
        if (packageName.contains("com.google.") || packageName.contains("com.kmxs.reader")) {
            doHookActivice(lpparam.classLoader);
        }

    }

    private boolean isTarget_hide(String name) {
        return name.contains("com.kmxs.reader") || name.contains("edxposed") || name.contains("lsposed") || name.contains("com.xuan.gamer") || name.contains("com.google.") || name.contains("magisk");
    }


    private void doHookPack(ClassLoader mClass) {
        try {
            try {
                XposedHelpers.findAndHookMethod(PackageItemInfo.class, "loadLabel", PackageManager.class, new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (loadLabel)");
                        //匹配想要修改的App的标签
                        if (param.getResult().equals("Loewe") || param.getResult().equals("EdXposed Manager") || param.getResult().equals("LSPosed") || param.getResult().equals("手游上号器") || param.getResult().equals("Magisk")) {
                            //param.setResult("腾讯加速器");
                            param.setResult(getRandomString(6));
                            XposedBridge.log("隐藏app_name");
                        }
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }

            try {
                XposedHelpers.findAndHookMethod("android.app.ApplicationPackageManager", mClass, "getInstalledApplications", int.class, new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getInstalledApplications)");
                        List<ApplicationInfo> applicationList = (List) param.getResult();
                        if (applicationList == null) return;

                        List<ApplicationInfo> resultapplicationList = new ArrayList<>();
                        for (ApplicationInfo applicationInfo : applicationList) {
                            String packageName = applicationInfo.packageName;
                            if (!isTarget_hide(packageName)) {
                                resultapplicationList.add(applicationInfo);
                            }
                        }
                        XposedBridge.log("隐藏getInstalledApplications");
                        param.setResult(resultapplicationList);
                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码01");
                e.printStackTrace();
            }
            try {
                XposedHelpers.findAndHookMethod("android.app.ApplicationPackageManager", mClass, "getInstalledPackages", int.class, new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getInstalledPackages)");
                        List<PackageInfo> packageInfoList = (List) param.getResult();
                        if (packageInfoList == null) return;

                        List<PackageInfo> resultpackageInfoList = new ArrayList<>();
                        for (PackageInfo packageInfo : packageInfoList) {
                            String packageName = packageInfo.packageName;
                            if (!isTarget_hide(packageName)) {
                                resultpackageInfoList.add(packageInfo);
                            }
                        }
                        XposedBridge.log("隐藏getInstalledPackages");
                        param.setResult(resultpackageInfoList);
                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码02");
                e.printStackTrace();
            }

            try {
                XposedHelpers.findAndHookMethod("android.app.ApplicationPackageManager", mClass, "getPackageInfo", String.class, int.class, new XC_MethodHook() {
                    @Override
                    protected void beforeHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getPackageInfo)");
                        List<PackageInfo> packageInfoList = (List) param.getResult();
                        if (packageInfoList == null) return;

                        List<PackageInfo> resultpackageInfoList = new ArrayList<>();
                        for (PackageInfo packageInfo : packageInfoList) {
                            String packageName = packageInfo.packageName;
                            if (!isTarget_hide(packageName)) {
                                resultpackageInfoList.add(packageInfo);
                            }
                        }
                        XposedBridge.log("隐藏getPackageInfo");
                        param.setResult(resultpackageInfoList);
                    }
                });
            } catch (Exception e) {
                e.printStackTrace();
            }
            try {
                XposedHelpers.findAndHookMethod("android.app.ApplicationPackageManager", mClass, "getApplicationInfo", String.class, int.class, new XC_MethodHook() {
                    @Override
                    protected void beforeHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getApplicationInfo)");
                        String packageName = (String) param.args[0];
                        if (packageName == null) return;
                        String packageName_ = "";
                        if (isTarget_hide(packageName)) {
                            param.setResult(packageName_);
                        }
                        XposedBridge.log("隐藏getApplicationInfo");

                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码03");
                e.printStackTrace();
            }
            try {
                XposedHelpers.findAndHookMethod("android.app.ActivityManager", mClass, "getRunningServices", int.class, new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getRunningServices)");
                        List<ActivityManager.RunningServiceInfo> serviceInfoList = (List) param.getResult();
                        if (serviceInfoList == null) return;
                        List<ActivityManager.RunningServiceInfo> resultList = new ArrayList<>();
                        for (ActivityManager.RunningServiceInfo runningServiceInfo : serviceInfoList) {
                            String serviceName = runningServiceInfo.process;
                            if (!isTarget_hide(serviceName)) {
                                resultList.add(runningServiceInfo);
                            }
                        }
                        XposedBridge.log("隐藏getRunningServices");
                        param.setResult(resultList);
                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码04");
                e.printStackTrace();
            }
            try {
                XposedHelpers.findAndHookMethod("android.app.ActivityManager", mClass, "getRunningTasks", int.class, new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getRunningTasks)");
                        List<ActivityManager.RunningTaskInfo> serviceInfoList = (List) param.getResult();
                        if (serviceInfoList == null) return;
                        List<ActivityManager.RunningTaskInfo> resultList = new ArrayList<>();
                        for (ActivityManager.RunningTaskInfo runningTaskInfo : serviceInfoList) {
                            String taskName = runningTaskInfo.baseActivity.flattenToString();
                            if (!isTarget_hide(taskName)) {
                                resultList.add(runningTaskInfo);
                            }
                        }
                        XposedBridge.log("隐藏getRunningTasks");
                        param.setResult(resultList);
                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码05");
                e.printStackTrace();
            }
            try {
                XposedHelpers.findAndHookMethod("android.app.ActivityManager", mClass, "getRunningAppProcesses", new XC_MethodHook() {
                    @Override
                    protected void afterHookedMethod(MethodHookParam param) {
                        XposedBridge.log("hide (getRunningAppProcesses)");
                        List<ActivityManager.RunningAppProcessInfo> runningAppProcessInfos = (List) param.getResult();
                        if (runningAppProcessInfos == null) return;
                        List<ActivityManager.RunningAppProcessInfo> resultList = new ArrayList<>();
                        for (ActivityManager.RunningAppProcessInfo runningAppProcessInfo : runningAppProcessInfos) {
                            String processName = runningAppProcessInfo.processName;
                            if (!isTarget_hide(processName)) {
                                resultList.add(runningAppProcessInfo);
                            }
                        }
                        XposedBridge.log("隐藏getRunningAppProcesses");
                        param.setResult(resultList);
                    }
                });
            } catch (Exception e) {
                XposedBridge.log("错误代码06");
                e.printStackTrace();
            }


        } catch (Exception e) {
            XposedBridge.log("错误代码999");
            e.printStackTrace();
        }


    }

    void doHookXposed() {
        try {
            // 过防止调用loadClass加载 de.robv.android.xposed.
            XposedHelpers.findAndHookMethod(ClassLoader.class, "loadClass", String.class, new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    if (param.args != null && param.args[0] != null) {
                        //XposedBridge.log("loadClass:" + param.args[0].toString());
                        if (param.args[0].toString().startsWith("de.robv.android.xposed.")) {
                            // 改成一个不存在的类
                            // param.args[0] = "de.robv.android.xposed.ThTest";
                            // 调整了一下，听说这样改更好，直接改部分手机有未知影响。
                            XposedBridge.log("试图访问de.robv.android.xposed 已拦截");
                            param.setThrowable(new ClassNotFoundException("not found"));
                        }

                        if (param.args[0].toString().contains("xposed") || param.args[0].toString().contains("simple")  || param.args[0].toString().contains("reader")) {
                            // 改成一个不存在的类
                            // param.args[0] = "de.robv.android.xposed.ThTest";
                            // 调整了一下，听说这样改更好，直接改部分手机有未知影响。
                            XposedBridge.log("试图访问私有类 已拦截");
                            param.setThrowable(new ClassNotFoundException("not found"));
                        }



                    }
                    super.beforeHookedMethod(param);
                }
            });
        } catch (Throwable t) {
        }

        try {
            //堆栈判断过滤xposed的内容
            XposedHelpers.findAndHookMethod(StackTraceElement.class, "getClassName", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    String result = (String) param.getResult();
                    if (result != null) {
                        //XposedBridge.log("堆栈:" + result);
                        if (result.contains("de.robv.android.xposed.") || result.contains("LspHooker") || result.contains("EdXposed")|| result.contains("xposed") || result.contains("google") || result.contains("reader")) {
                            param.setResult("");
                            XposedBridge.log("堆栈判断过滤");

                            // Log.i(tag, "替换了，字符串名称 " + result);
                        } else if (result.contains("com.android.internal.os.ZygoteInit")) {
                            param.setResult("");
                            XposedBridge.log("堆栈判断过滤2");
                        }
                    }

                    super.afterHookedMethod(param);
                }
            });
        } catch (Throwable t) {
        }
        try {
            //进程中过滤xposed模块
            XposedHelpers.findAndHookMethod(BufferedReader.class, "readLine", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    String result = (String) param.getResult();
                    if (result != null) {
                        // XposedBridge.log("进程:" + result);
                        if (result.contains("/data/data/de.robv.android.xposed.installer/bin/XposedBridge.jar")) {
                            param.setResult("");
                            new File("").lastModified();
                            XposedBridge.log("xposed进程过滤");
                        }
                        if (result.contains("com.kmxs.reader") || result.contains("edxposed") || result.contains("lsposed") || result.contains("com.xuan.gamer") || result.contains("com.google.") || result.contains("magisk")){
                            param.setResult("");
                            new File("").lastModified();
                            XposedBridge.log("进程过滤2");
                        }
                    }

                    super.afterHookedMethod(param);
                }
            });
        } catch (Throwable t) {
        }


    }

    private void doHookActivice(ClassLoader mClassLoader) {
        try {


            XposedHelpers.findAndHookMethod(
                    "me.simple.lib.utils.ShellUtils",
                    mClassLoader,
                    "isActivice",
                    new XC_MethodReplacement() {
                        @Override
                        protected Object replaceHookedMethod(MethodHookParam param) {
                            return true;
                        }
                    });
        } catch (Throwable th) {
            XposedBridge.log("该app未找到这个类:ShellUtils");

        }
    }

    private void afterApplicationPug(ClassLoader mClassLoader) {
        try {
            XposedHelpers.findAndHookMethod(Application.class, "onCreate", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    Context appContext = (Context) ReflectUtils.rejectFind(param, "java.lang.Object", "thisObject");

                    try {
                        hookSercue();
                    } catch (Throwable th) {
                        XposedBridge.log(th);
                    }
                    try {
                        doHookInput(appContext);
                    } catch (Throwable th) {

                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void hookSercue() {
        try {
            Method method = ReflectUtils.findMethod(Settings.Secure.class, "getInt", 3);
            XposedBridge.hookMethod(method, new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                }

                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    XposedBridge.log("检查开发者模式");

                    String params1 = (String) param.args[1];
                    if (Settings.Secure.ADB_ENABLED.equals(params1)) {
                        param.setResult(0);
                    }
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void doHookInput(Context context) {
        try {
            Class<?> mContextImpl = XposedHelpers.findClass("android.hardware.input.InputManager", context.getClassLoader());
            XposedBridge.hookAllMethods(mContextImpl, "getInputDeviceIds", new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                }

                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    XposedBridge.log("InputManager:检查触摸设备02");
                    //Log.d("InputManager", "检查触摸设备02");
                    //int[] devIds = inputManager.getInputDeviceIds();
                    int[] devIds = (int[]) param.getResult();
                    ArrayList<Integer> ret = new ArrayList<>();
                    for (int devId : devIds) {
                        InputManager inputManager = (InputManager) context.getSystemService(Context.INPUT_SERVICE);
                        InputDevice device = inputManager.getInputDevice(devId);
                        if (device.getName() != null && !device.getName().equals("event_yhds")) {
                            //        XposedBridge.log("Input device name : " + device.getName() + "---- info: " + device.toString());
                            ret.add(devId);
                        }
                    }
                    int[] results = new int[ret.size()];
                    for (int i = 0; i < ret.size(); i++) {
                        results[i] = ret.get(i);
                    }
                    param.setResult(results);
                }
            });
        } catch (XposedHelpers.ClassNotFoundError e) {
            e.printStackTrace();
        }

        /**
         * android.view.InputDevice
         */

        try {
            Class<?> mContextImpl = XposedHelpers.findClass("android.view.InputDevice", context.getClassLoader());
            XposedBridge.hookAllMethods(mContextImpl, "getDeviceIds", new XC_MethodHook() {

                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                }

                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    XposedBridge.log("InputDevice:检查触摸设备01");
                    int[] result = (int[]) param.getResult();
                    ArrayList<Integer> ret = new ArrayList<>();
                    for (int id : result) {
                        InputDevice mInputDevice = InputDevice.getDevice(id);
                        String mName = mInputDevice.getName();
                        //  XposedBridge.log("neme:" + mName + " id:" + id + " Result:" + param.getResult());
                        if (mName != null && !mName.equals("event_yhds")) {
                            ret.add(id);
                        }
                    }
                    int[] results = new int[ret.size()];
                    for (int i = 0; i < ret.size(); i++) {
                        results[i] = ret.get(i);
                    }
                    param.setResult(results);
                }
            });
        } catch (XposedHelpers.ClassNotFoundError e) {
            e.printStackTrace();
        }


    }

    public static void doHookMediaProjection() {

        try {
            XposedHelpers.findAndHookMethod(MediaProjectionManager.class, "getMediaProjection", int.class, Intent.class, new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    {
                        try {
                            XposedBridge.log("游戏试图录屏,已拦截");
                            param.setResult(null);

                        } catch (Throwable th) {
                            XposedBridge.log(th);
                        }
                    }
                }
            });
        } catch (Exception th) {
            XposedBridge.log(th);
        }
    }

    public static void doHookIntExtra(Context context) {
        try {
            Class<?> mContextImpl = XposedHelpers.findClass("android.content.Intent", context.getClassLoader());
            XposedBridge.hookAllMethods(mContextImpl, "getIntExtra", new XC_MethodHook() {
                @Override
                protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                    super.beforeHookedMethod(param);
                }

                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    //  XposedBridge.log("检测USB:检测USB");
                    //  Log.d("IntExtra", "检测USB");
                    param.setResult(Intent.EXTRA_DOCK_STATE_DESK);
                }
            });
        } catch (XposedHelpers.ClassNotFoundError e) {
            e.printStackTrace();
        }


    }

    private void doHookSettings(ClassLoader mClassLoader) {
        try {

            XposedHelpers.findAndHookMethod(
                    "android.provider.Settings.Secure",
                    mClassLoader,
                    "getString",
                    ContentResolver.class,
                    String.class,
                    new XC_MethodHook() {
                        @Override
                        protected void beforeHookedMethod(MethodHookParam param) {
                            XposedBridge.log("Secure：androidId:" + settingsId);
                            // Log.d("Secure：androidId:", id);
                            param.setResult(settingsId);
                        }
                    });

            XposedHelpers.findAndHookMethod(
                    "android.provider.Settings.System",
                    mClassLoader,
                    "getString",
                    ContentResolver.class,
                    String.class,
                    new XC_MethodHook() {
                        @Override
                        protected void beforeHookedMethod(MethodHookParam param) {
                            XposedBridge.log("System：androidId:" + settingsId);
                            //  Log.d("System：androidId:", id);
                            param.setResult(settingsId);
                        }
                    });
        } catch (XposedHelpers.ClassNotFoundError e) {
            e.printStackTrace();
        }
    }

    public static void doHookDevice(Context context) {
        try {

            Class<?> mContextImpl = XposedHelpers.findClass("android.telephony.TelephonyManager", context.getClassLoader());
            XposedBridge.hookAllMethods(mContextImpl, "getDeviceId", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);

                    XposedBridge.log("随机：DeviceId:" + id);
                    param.setResult(id);
                }
            });

            XposedBridge.hookAllMethods(mContextImpl, "getMeid", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);

                    XposedBridge.log("随机：Meid:" + id);
                    param.setResult(id);
                }
            });

            XposedBridge.hookAllMethods(mContextImpl, "getSubscriberId", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);
                    XposedBridge.log("随机：getSubscriberId:" + id);
                    param.setResult(id);
                }
            });


            XposedBridge.hookAllMethods(mContextImpl, "getSimSerialNumber", new XC_MethodHook() {
                @Override
                protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                    super.afterHookedMethod(param);

                    XposedBridge.log("随机：SimSerialNumber:" + id);
                    param.setResult(id);
                }
            });
        } catch (XposedHelpers.ClassNotFoundError e) {
            e.printStackTrace();
        }

    }

    public static String getRandomString(int length) {
        String base = "abcdefghijklmnopqrstuvwxyz0123456789";
        Random random = new Random();
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < length; i++) {
            int number = random.nextInt(base.length());
            sb.append(base.charAt(number));
        }
        return sb.toString();
    }

    public static int getRandomInt(int length) {
        String base = "0123456789";
        Random random = new Random();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < length; i++) {
            int number = random.nextInt(base.length());
            sb.append(base.charAt(number));
        }
        return Integer.parseInt(sb.toString());
    }


}




