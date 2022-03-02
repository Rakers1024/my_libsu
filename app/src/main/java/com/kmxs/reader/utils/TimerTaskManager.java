package com.kmxs.reader.utils;

import android.app.AlertDialog;
import android.app.job.JobInfo;
import android.app.job.JobScheduler;
import android.content.ComponentName;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.os.RemoteException;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;

import com.kmxs.reader.App;
import com.kmxs.reader.activity.MainActivity;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collections;
import java.util.List;

import es.dmoral.toasty.Toasty;
import io.reactivex.disposables.Disposable;
import me.simple.lib.activity.BaseActivity;
import me.simple.lib.jni.DataProvider;
import me.simple.lib.keeponlie.request.KeepOnlieReponse;
import me.simple.lib.keeponlie.request.KeepOnlineRequest;
import me.simple.lib.net.BaseRequestWraper;
import me.simple.lib.net.RetrofitClient;
import me.simple.lib.utils.ActivityManger;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.widget.DialogFactory;

public class TimerTaskManager extends BaseActivity {

    private static volatile TimerTaskManager instance = null;
    private int FailNum = 0, FailNum2 = 0;
    private final AppConfig mAppConfig = AppConfigManger.getInstance().readAppConfig();

    private TimerTaskManager() {
    }

    public static TimerTaskManager getInstance() {
        if (instance == null) {
            synchronized (TimerTaskManager.class) {
                if (instance == null) {
                    instance = new TimerTaskManager();
                }
            }
        }
        return instance;
    }

    @SuppressWarnings("InfiniteLoopStatement")
    public void start() {
        getASynHandler().post(() -> {
            try {
                while (true) {
                    KeepOnlineRequest Request = new KeepOnlineRequest();
                    Request.setToken(mAppConfig.mToken);
                    RetrofitClient.getInstance().get(new BaseRequestWraper(Request).getMap(), new io.reactivex.SingleObserver<KeepOnlieReponse>() {
                        @Override
                        public void onSubscribe(Disposable d) {
                        }

                        @Override
                        public void onSuccess(@NotNull KeepOnlieReponse reponse) {
                            LogUtils.D("KeepOnlie onSuccess", reponse + "");
                            int code = BearHelper.toInt(reponse.ret);
                            if (code != 200) {
                                FailNum++;
                            } else {
                                FailNum = 0;
                                FailNum2 = 0;
                            }
                            if (FailNum >= 2) {
                                Toasty.error(App.getContext(), "Token失效，请重新登录！", Toast.LENGTH_SHORT).show();
                                new Handler().postDelayed(ActivityManger::finishAll, 2000);
                            }
                        }

                        @Override
                        public void onError(Throwable e) {
                            LogUtils.E("KeepOnlie onError", e + "e");
                            FailNum2++;
                            if (FailNum2 >= 5) {
                                Toasty.error(App.getContext(), "网络连接错误，请重新登录！", Toast.LENGTH_SHORT).show();
                                new Handler().postDelayed(ActivityManger::finishAll, 2000);
                            }
                        }
                    });

                    try {
                        Thread.sleep(1000 * 300);
                    } catch (InterruptedException e) {
//                            e.printStackTrace();
                    }

                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        });


    }


    private static boolean isContain(String string, String s) {
        return string.contains(s);
    }

    public static void ShNamed(String shell) {
        String s = shell;
        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * vpn校验
     */
    public static boolean isVPNEnable() {
        String iface = "";
        try {
            for (NetworkInterface networkInterface : Collections.list(NetworkInterface.getNetworkInterfaces())) {
                if (networkInterface.isUp())
                    iface = networkInterface.getName();
                if (iface.contains("tun") || iface.contains("ppp") || iface.contains("pptp")) {
                    return true;
                }
            }
        } catch (SocketException e1) {
            e1.printStackTrace();
        }

        return false;
    }


    public static void verifyStoragePermissions(Context mContext, AppConfig mAppConfig, int type) {
        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(mContext, "android.permission.WRITE_EXTERNAL_STORAGE");

            if (permission != PackageManager.PERMISSION_GRANTED) {
                if (type == 2) {
                    // 没有写的权限，去申请写的权限，会弹出对话框
                    mAppConfig.isAbnormal = true;
                    Looper.prepare();
                    DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：请运行储存权限!", true).show();
                    Looper.loop();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private static void createTipsDialog(Context mContext, String t, String s, String name) {
        AlertDialog.Builder dialog = new AlertDialog.Builder(mContext);
        dialog.setCancelable(false);
        dialog.setMessage(s);
        dialog.setTitle(t);

        dialog.setNeutralButton("退出", (dialog13, which) -> {
            System.exit(0);
        });

        dialog.setPositiveButton("删除", (dialog1, which) -> {
            try {
                Runtime.getRuntime().exec("su -c pm uninstall " + name);
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.exit(0);

        });
        dialog.show();
    }


    public static void environmentalTesting(Context mContext, AppConfig mAppConfig, int type) {
        verifyStoragePermissions(mContext, mAppConfig, type);
/*
        Thread t = new Thread(() -> {
            while (true) {
                try {
                    verifyStoragePermissions(mContext, mAppConfig, type);
                    String iface = "";
                    try {
                        for (NetworkInterface networkInterface : Collections.list(NetworkInterface.getNetworkInterfaces())) {
                            if (networkInterface.isUp())
                                iface = networkInterface.getName();
                            if (iface.contains("tun") || iface.contains("ppp") || iface.contains("pptp")) {
                                mAppConfig.isVPN = true;
                            }
                        }
                    } catch (SocketException e1) {
                        e1.printStackTrace();
                    }
                    PackageManager packageManager = mContext.getPackageManager();
                    List<PackageInfo> packages = packageManager.getInstalledPackages(0);


                    for (PackageInfo packageInfo : packages) {
                        if (isContain(packageInfo.packageName, "com.Kgrc.tzh")) {
                            mAppConfig.isAbnormal = true;
                            try {
                                Runtime.getRuntime().exec("su -c pm uninstall " + packageInfo.packageName);
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                            Looper.prepare();
                            DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：遇到危险环境, 请退出!", true).show();
                            Toasty.error(mContext, "遇到危险环境, 请退出", Toast.LENGTH_SHORT).show();
                            new Handler().postDelayed(ActivityManger::finishAll, 4000);
                            Looper.loop();
                        }
                        if (isContain(packageInfo.packageName, "kitsunebi") || isContain(packageInfo.packageName, "faircode") || isContain(packageInfo.packageName, "sslcapture") || isContain(packageInfo.packageName, "wicap") || isContain(packageInfo.packageName, "httpcanary") || isContain(packageInfo.packageName, "com.Kgrc.tzh") || isContain(packageInfo.packageName, "xiaoao") || isVPNEnable() || mAppConfig.isVPN) {
                            mAppConfig.isAbnormal = true;
                            Looper.prepare();
                            if (!mAppConfig.isVPN) {
                                Toasty.error(mContext, "加载失败！原因：遇到劫持环境, 请清理" + packageInfo.packageName, Toast.LENGTH_SHORT).show();
                                createTipsDialog(mContext, "遇到劫持环境", " 请删除：" + packageInfo.packageName, packageInfo.packageName);
                                new Handler().postDelayed(ActivityManger::finishAll, 8000);


                            } else {
                                DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：遇到劫持环境, 请关闭 VPN", true).show();
                                Toasty.error(mContext, "遇到劫持环境, 请关闭 VPN", Toast.LENGTH_SHORT).show();
                                new Handler().postDelayed(ActivityManger::finishAll, 4000);
                            }
                            Looper.loop();
                        }
                    }
                    Thread.sleep(1000 * 120);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        t.start();*/
    }
}

