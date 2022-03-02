package com.kmxs.reader.activity;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import com.kmxs.reader.App;
import com.kmxs.reader.R;
import com.kmxs.reader.core.Config;
import com.topjohnwu.superuser.io.SuFile;
import com.topjohnwu.superuser.io.SuFileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import es.dmoral.toasty.Toasty;
import me.simple.lib.activity.BaseActivity;
import me.simple.lib.injection.RootUtil;
import me.simple.lib.utils.CustomDialog;
import me.simple.lib.utils.CustomProgressDialog;
import me.simple.lib.utils.ShellUtils;
import static com.kmxs.reader.activity.MainActivity.MODE_NAME;

public class ModuleInof extends BaseActivity {

    private Context mContext;

    @SuppressWarnings("deprecation")
    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.module_info);
        mContext = this;
        try {
            initView();
        } catch (Exception e) {
            e.printStackTrace();
        }

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

    CustomProgressDialog mProgressDialog;

    private CustomProgressDialog createProgressDialog(String title, String message) {
        CustomProgressDialog dialog = new CustomProgressDialog(mContext);
        dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        dialog.setTitle(title);
        dialog.setMessage(message);
        dialog.setCancelable(true);
        dialog.setCanceledOnTouchOutside(false);
        return dialog;
    }

    private final Handler mHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == 0) {
                mProgressDialog.dismiss();
                CustomDialog mDialog = new CustomDialog(mContext);
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

    /**
     * 安装模块
     */
    public void installModules() {
        mProgressDialog = createProgressDialog("请等待一会...", "正在加载...");
        mProgressDialog.setCancelable(false);
        mProgressDialog.show();
        loadAssets(MODE_NAME + ".zip");
        RootUtil.runShell("magisk --install-module " + getFilesDir().getAbsolutePath() + "/" + MODE_NAME + ".zip", true);
        //清理
        RootUtil.runShell("/system/bin/rm -r " + App.getFilePath(), true);
        Toasty.success(mContext, "需要确认您安装了Riru模块", Toast.LENGTH_LONG).show();
        mProgressDialog.dismiss();
        tv.setText("安装完成");
        status_summary.setText("请重启手机");
        relativeLayout.setBackgroundResource(R.drawable.hook_true);
        status_icon.setBackgroundResource(R.drawable.ic_round_check_circle_24);

        mHandler.sendEmptyMessage(0);
        Toasty.success(mContext, "配置环境结束", Toast.LENGTH_LONG).show();
    }

    TextView tv;
    ImageView status_icon;
    TextView status_summary;
    RelativeLayout relativeLayout;

    private void initView() throws Exception {

        tv = findViewById(R.id.status_title);
        status_summary = findViewById(R.id.status_summary);
        relativeLayout = findViewById(R.id.safe);
        status_icon = findViewById(R.id.status_icon);

        TextView device = findViewById(R.id.device);
        device.setText(Build.MODEL);
        TextView system_version = findViewById(R.id.system_version);
        system_version.setText(Build.VERSION.RELEASE + " (" + Build.VERSION.SDK_INT + ")");

        TextView framework_version = findViewById(R.id.framework_version);
        try {
            framework_version.setText(getVersionName(getPackageName()));
        } catch (Exception e) {
            e.printStackTrace();
        }

        TextView manager_version = findViewById(R.id.manager_version);
        try {
            manager_version.setText(getVersionName("org.lsposed.manager"));
        } catch (Exception e) {
            e.printStackTrace();
        }


        TextView magisk_version = findViewById(R.id.magisk_version);
        try {
            magisk_version.setText(getMagiskVersionCode() + "(" + getMagiskVersion() + ")");
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (Config.isModuleInstall && Config.isModuleUpdate) {
            tv.setText("已激活");
            status_summary.setText("开始奔放吧!!");
            relativeLayout.setBackgroundResource(R.drawable.hook_true);
            status_icon.setBackgroundResource(R.drawable.ic_round_check_circle_24);
        } else if (!Config.isModuleInstall) {
            tv.setText("模块未安装");
            status_summary.setText("点我安装");
            status_icon.setBackgroundResource(R.drawable.ic_round_warning_24);
            relativeLayout.setBackgroundResource(R.drawable.hook_false);
            relativeLayout.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    //安装模块
                    installModules();
                }
            });
        } else {
            tv.setText("模块有更新");
            status_summary.setText("点我安装");
            status_icon.setBackgroundResource(R.drawable.ic_round_warning_24);
            relativeLayout.setBackgroundResource(R.drawable.hook_false);
            relativeLayout.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    //安装模块
                    installModules();

                }
            });
        }
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

    public static String getMagiskVersion() {
        int i;
        SuFile suFile = new SuFile("/data/adb/magisk/util_functions.sh");
        if (suFile.exists()) {
            try {
                SuFileInputStream suFileInputStream = new SuFileInputStream(suFile);
                byte[] bArr = new byte[1024];
                String str = "";
                while (true) {
                    int read = suFileInputStream.read(bArr);
                    if (read == -1) {
                        break;
                    }
                    str = str + new String(bArr, 0, read);
                }
                suFileInputStream.close();
                for (String str2 : str.split("\n")) {
                    if (str2.contains("MAGISK_VER=")) {
                        return str2.replace("MAGISK_VER=", "").replace("'", "");
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return "";
    }

    public static int getMagiskVersionCode() {
        int i;
        SuFile suFile = new SuFile("/data/adb/magisk/util_functions.sh");
        if (suFile.exists()) {
            try {
                SuFileInputStream suFileInputStream = new SuFileInputStream(suFile);
                byte[] bArr = new byte[1024];
                String str = "";
                while (true) {
                    int read = suFileInputStream.read(bArr);
                    if (read == -1) {
                        break;
                    }
                    str = str + new String(bArr, 0, read);
                }
                suFileInputStream.close();
                for (String str2 : str.split("\n")) {
                    if (str2.contains("MAGISK_VER_CODE")) {
                        return Integer.parseInt(str2.replace("MAGISK_VER_CODE=", ""));
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return -1;
    }

    public static String getMagiskMoudleVersion(String str) {
        int i;
        SuFile suFile = new SuFile("/data/adb/modules/" + str + "/module.prop");
        if (suFile.exists()) {
            try {
                SuFileInputStream suFileInputStream = new SuFileInputStream(suFile);
                byte[] bArr = new byte[1024];
                String str2 = "";
                while (true) {
                    int read = suFileInputStream.read(bArr);
                    if (read == -1) {
                        break;
                    }
                    str2 = str2 + new String(bArr, 0, read);
                }
                suFileInputStream.close();
                for (String str3 : str2.split("\n")) {
                    if (str3.contains("version=")) {
                        return str3.replace("version=", "");
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return "";
    }

    public static int getMagiskMoudleVersionCode(String str) {
        int i;
        SuFile suFile = new SuFile("/data/adb/modules/" + str + "/module.prop");
        if (suFile.exists()) {
            try {
                SuFileInputStream suFileInputStream = new SuFileInputStream(suFile);
                byte[] bArr = new byte[1024];
                String str2 = "";
                while (true) {
                    int read = suFileInputStream.read(bArr);
                    if (read == -1) {
                        break;
                    }
                    str2 = str2 + new String(bArr, 0, read);
                }
                suFileInputStream.close();
                for (String str3 : str2.split("\n")) {
                    if (str3.contains("versionCode")) {
                        return Integer.parseInt(str3.replace("versionCode=", ""));
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return -1;
    }


}
