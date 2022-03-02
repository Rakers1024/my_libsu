package com.kmxs.reader.utils;

import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.AppOpsManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.Build;
import android.os.PowerManager;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;

import com.google.android.material.textfield.TextInputEditText;
import com.google.android.material.textfield.TextInputLayout;
import com.kmxs.reader.App;
import com.kmxs.reader.R;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.zip.ZipEntry;

import me.simple.lib.utils.CustomDialog;
import me.simple.lib.utils.DeviceInfoUtils;
import me.simple.lib.utils.EncrypUtils;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.SafeUtils;
import me.simple.lib.utils.ShellUtils;

public class BearHelper {

    @SuppressLint("StaticFieldLeak")
    private static final Context mContext = App.getContext();
    private static final String TAG = "BearHelper";

    @RequiresApi(api = Build.VERSION_CODES.M)
    public static void allowAllPermissions() {
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

    public static boolean isServiceRunning(String ServiceName) {
        if (("").equals(ServiceName) || ServiceName == null)
            return false;
        ActivityManager myManager = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
        ArrayList<ActivityManager.RunningServiceInfo> runningService = (ArrayList<ActivityManager.RunningServiceInfo>) myManager.getRunningServices(30);
        for (int i = 0; i < runningService.size(); i++) {
            if (runningService.get(i).service.getClassName().toString().equals(ServiceName)) {
                return true;
            }
        }
        return false;
    }

    public static boolean CheckOp(int op) {
        try {
            AppOpsManager appOpsManager = (AppOpsManager) mContext.getSystemService(Context.APP_OPS_SERVICE);
            if (appOpsManager == null) {
                return false;
            }
            Class<? extends AppOpsManager> localClass = appOpsManager.getClass();
            Class[] arra = new Class[3];
            arra[0] = Integer.TYPE;
            arra[1] = Integer.TYPE;
            arra[2] = String.class;
            Method method = localClass.getMethod("checkOp", arra);
            if (method == null) {
                return false;
            }
            Object[] arra1 = new Object[3];
            arra1[0] = op;
            arra1[1] = Binder.getCallingUid();
            arra1[2] = mContext.getPackageName();
            try {
                return (int)method.invoke(appOpsManager, arra1) == AppOpsManager.MODE_ALLOWED;
            } catch (InvocationTargetException | IllegalAccessException | IllegalArgumentException e) {
                e.printStackTrace();
                return false;
            }
        } catch (SecurityException | NoSuchMethodException e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean isIgnoringBatteryOptimizations() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            PowerManager powerManager = (PowerManager) mContext.getSystemService(Context.POWER_SERVICE);
            return powerManager.isIgnoringBatteryOptimizations(mContext.getPackageName());
        }
        return false;
    }

    @Nullable
    public static InputStream openAssets(String dir, String name) {
        InputStream in = null;
        int i = dir.length() + 1;
        try {
            java.util.zip.ZipFile zip = new java.util.zip.ZipFile(mContext.getApplicationInfo().publicSourceDir);
            Enumeration<? extends ZipEntry> entries = zip.entries();
            while (entries.hasMoreElements()) {
                ZipEntry entry = entries.nextElement();
                String temp = entry.getName();
                if (temp.indexOf(dir) != 0)
                    continue;
                String path = temp.substring(i);
                if (!entry.isDirectory()) {
                    if (path.equals(name)) {
                        in = zip.getInputStream(entry);
                    }
                }
            }
            // zip.close();
            return in;
        } catch (IOException e) {
            return null;
        }
    }

    public static void WriteAssets(String InFileName, String OutFileName) {
        try {
            OutputStream myOutput = new FileOutputStream(OutFileName);
            InputStream myInput = openAssets("assets", InFileName);
            byte[] buffer = new byte[4096];
            int length = myInput.read(buffer);
            while (length > 0) {
                myOutput.write(buffer, 0, length);
                length = myInput.read(buffer);
            }
            myOutput.flush();
            myInput.close();
            myOutput.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public static byte[] toByte(String v, String encoding) {
        try {
            return v.getBytes(encoding);
        } catch (UnsupportedEncodingException e) {
            return new byte[0];
        }
    }

    public static long toLong(String v) {
        try {
            if (!"".equals(v)) {
                return Long.parseLong(v);
            }
            return 0;
        } catch (Exception e) {
            return 0;
        }
    }

    public static float toNumber(String v) {
        try {
            if (!TextUtils.isEmpty(v)) {
                return Float.parseFloat(v);
            }
            return 0.0f;
        } catch (Exception e) {
            return 0.0f;
        }
    }

    public static int toInt(String v) {
        try {
            if (!"".equals(v)) {
                return Integer.parseInt(v);
            }
            return 0;
        } catch (Exception e) {
            return 0;
        }
    }

    /* 获取imei */
    public static String getDeviceId() {
        String imei = App.ReadConfig("deviceid","");
        if (!TextUtils.isEmpty(imei)){
            return imei;
        }
        imei = DeviceInfoUtils.getDeviceId(null);
        App.SaveConfig("deviceid", EncrypUtils.oOOOooooOOoOOoo(imei, SafeUtils.rc4key));
        return EncrypUtils.oOOOooooOOoOOoo(imei, SafeUtils.rc4key);
    }

    public interface onDialogReslutLisner {
        void onReslut(String value,String value2);
        //void onFailure(Object result);
    }


    public static void rawInput(Context context,String title, String value,String hint,String value2,String hint2, final onDialogReslutLisner callback) {
        @SuppressLint("InflateParams")
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_input, null);
        CustomDialog dialog = new CustomDialog(context);
        dialog.setTitle(title);
        dialog.setView(view);
        dialog.setType(CustomDialog.TYPE_FLOAT);
        TextInputLayout mTextInputLayout = view.findViewById(R.id.mTextInputLayout);
        mTextInputLayout.setHint(hint);
        final TextInputEditText mEditText = view.findViewById(R.id.mEditText);
        mEditText.setSingleLine(true);
        mEditText.setText(value);


        TextInputLayout mTextInputLayout1 = view.findViewById(R.id.mTextInputLayout1);
        final TextInputEditText mEditText1 = view.findViewById(R.id.mEditText1);
        mEditText1.setSingleLine(true);
        if (value2 == null && hint2 == null){
            mTextInputLayout1.setVisibility(View.GONE);
        }else{
            mEditText1.setText(value2);
            mTextInputLayout1.setHint(hint2);
        }

        dialog.setButton(DialogInterface.BUTTON_POSITIVE, "确定", (dialog1, which) -> {
            callback.onReslut(mEditText.getText().toString(),mEditText1.getText().toString());
            dialog1.dismiss();
        });
        dialog.show();
    }

    public static void rawInput2(Context context,String title, String value,String hint,String value2,String hint2, final onDialogReslutLisner callback) {
        @SuppressLint("InflateParams")
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_input, null);
        CustomDialog dialog = new CustomDialog(context);
        dialog.setTitle(title);
        dialog.setView(view);

        TextInputLayout mTextInputLayout = view.findViewById(R.id.mTextInputLayout);
        final TextInputEditText mEditText = view.findViewById(R.id.mEditText);
        mEditText.setText(value);
        mTextInputLayout.setHint(hint);

        TextInputLayout mTextInputLayout1 = view.findViewById(R.id.mTextInputLayout1);
        final TextInputEditText mEditText1 = view.findViewById(R.id.mEditText1);

        if (value2 == null && hint2 == null){
            mTextInputLayout1.setVisibility(View.GONE);
        }else{
            mEditText1.setText(value2);
            mTextInputLayout1.setHint(hint2);
        }

        dialog.setButton(DialogInterface.BUTTON_POSITIVE, "确定", (dialog1, which) -> {
            callback.onReslut(mEditText.getText().toString(),mEditText1.getText().toString());
            dialog1.dismiss();
        });
        dialog.show();
    }
}
