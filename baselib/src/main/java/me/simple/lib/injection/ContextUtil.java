package me.simple.lib.injection;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Looper;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class ContextUtil {
    public static Application getApplicationContext() {
        try {
            Class<?> activityThreadClass = Class.forName("android.app.ActivityThread");
//            Method currentActivityThreadMethod = activityThreadClass.getDeclaredMethod("currentActivityThread");
//            currentActivityThreadMethod.setAccessible(true);
//            Object activityThread = currentActivityThreadMethod.invoke(activityThreadClass);
            Method currentApplicationMethod = activityThreadClass.getDeclaredMethod("currentApplication");
            currentApplicationMethod.setAccessible(true);
            Application application = (Application) currentApplicationMethod.invoke(activityThreadClass);
            if (application.getApplicationContext() instanceof Application) {
                return (Application) application.getApplicationContext();
            }
            return application;
        } catch (Exception e) {
            return null;
        }
    }


    public static Activity getCurrentActivity() {
        Class activityThreadClass = null;
        try {
            activityThreadClass = Class.forName("android.app.ActivityThread");
            Object activityThread = activityThreadClass.getMethod("currentActivityThread").invoke(null);
            Field activitiesField = activityThreadClass.getDeclaredField("mActivities");
            activitiesField.setAccessible(true);
            Map activities = (Map) activitiesField.get(activityThread);
            for (Object activityRecord : activities.values()) {
                Class activityRecordClass = activityRecord.getClass();
                Field pausedField = activityRecordClass.getDeclaredField("paused");
                pausedField.setAccessible(true);
                if (!pausedField.getBoolean(activityRecord)) {
                    Field activityField = activityRecordClass.getDeclaredField("activity");
                    activityField.setAccessible(true);
                    Activity activity = (Activity) activityField.get(activityRecord);
                    return activity;
                }
            }
        } catch (Exception e) {
            return null;
        }
        return null;
    }

    public static List<Activity> getCreatedActivities() {
        List<Activity> activityList = new ArrayList<Activity>();
        Class activityThreadClass = null;
        try {
            activityThreadClass = Class.forName("android.app.ActivityThread");
            Object activityThread = activityThreadClass.getMethod("currentActivityThread").invoke(null);
            Field activitiesField = activityThreadClass.getDeclaredField("mActivities");
            activitiesField.setAccessible(true);
            Map activities = (Map) activitiesField.get(activityThread);
            for (Object activityRecord : activities.values()) {
                Class activityRecordClass = activityRecord.getClass();
//                Field pausedField = activityRecordClass.getDeclaredField("paused");
//                pausedField.setAccessible(true);
//                if (!pausedField.getBoolean(activityRecord)) {
                    Field activityField = activityRecordClass.getDeclaredField("activity");
                    activityField.setAccessible(true);
                    Activity activity = (Activity) activityField.get(activityRecord);
//                }
                activityList.add(activity);
            }
        } catch (Exception e) {
        }
        return activityList;
    }

    public static Context getPluginContext(Context useContext){
        try {
            return useContext.createPackageContext(Params.MYOACKAGE, Context.CONTEXT_IGNORE_SECURITY | Context.CONTEXT_INCLUDE_CODE | Context.CONTEXT_RESTRICTED);
        } catch (PackageManager.NameNotFoundException e) {
            return null;
        }
    }

    public static Context getGameContext(Context useContext){
        try {
            return useContext.createPackageContext(Params.GAMEPACKAGE, Context.CONTEXT_IGNORE_SECURITY | Context.CONTEXT_INCLUDE_CODE | Context.CONTEXT_RESTRICTED);
        } catch (PackageManager.NameNotFoundException e) {
            return null;
        }
    }

    private static final Object lock = new Object();
    private static Context systemContext = null;
    public static Context getSystemContext() {
        synchronized (lock) {
            try {
                if (systemContext != null) {
                    return systemContext;
                }

                // a prepared Looper is required for the calls below to succeed
                if (Looper.getMainLooper() == null) {
                    try {
                        Looper.prepareMainLooper();
                    } catch (Exception e) {
                    }
                }

                Class<?> cActivityThread = Class.forName("android.app.ActivityThread");
                Method mSystemMain = cActivityThread.getMethod("systemMain");
                Method mGetSystemContext = cActivityThread.getMethod("getSystemContext");

                Object oActivityThread = mSystemMain.invoke(null);
                Object oContext = mGetSystemContext.invoke(oActivityThread);

                systemContext = (Context)oContext;
                return systemContext;
            } catch (Exception e) {
                throw new RuntimeException("librootjava: unexpected exception in getSystemContext()");
            }
        }
    }
}
