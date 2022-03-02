package com.kmxs.reader.activity;

import android.annotation.TargetApi;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Toast;

import androidx.annotation.RequiresApi;

import com.kmxs.reader.R;
import com.kmxs.reader.core.Config;

import java.io.IOException;

import es.dmoral.toasty.Toasty;
import me.simple.lib.activity.BaseActivity;
import me.simple.lib.injection.RootUtil;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ZipUtil;
import me.simple.lib.widget.DialogFactory;

public class SettingActivity extends BaseActivity {


    private Context mContext;
    public static String dataDir;

    @SuppressWarnings("deprecation")
    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.acticity_setting);
        mContext = this;
        try {
            initView();
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * 专为Android4.4设计的从Uri获取文件绝对路径，以前的方法已不好使
     */
    public static String filePath;

    @TargetApi(Build.VERSION_CODES.KITKAT)
    public String getPath(final Context context, final Uri uri) {

        final boolean isKitKat = Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;

        // DocumentProvider
        if (isKitKat && DocumentsContract.isDocumentUri(context, uri)) {
            // ExternalStorageProvider
            if (isExternalStorageDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                if ("primary".equalsIgnoreCase(type)) {
                    return Environment.getExternalStorageDirectory() + "/" + split[1];
                }
            }
            // DownloadsProvider
            else if (isDownloadsDocument(uri)) {

                final String id = DocumentsContract.getDocumentId(uri);
                final Uri contentUri = ContentUris.withAppendedId(
                        Uri.parse("content://downloads/public_downloads"), Long.valueOf(id));

                return getDataColumn(context, contentUri, null, null);
            }

            // MediaProvider
            else if (isMediaDocument(uri)) {
                final String docId = DocumentsContract.getDocumentId(uri);
                final String[] split = docId.split(":");
                final String type = split[0];

                Uri contentUri = null;
                if ("image".equals(type)) {
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                } else if ("video".equals(type)) {
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                } else if ("audio".equals(type)) {
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                }

                final String selection = "_id=?";
                final String[] selectionArgs = new String[]{split[1]};

                return getDataColumn(context, contentUri, selection, selectionArgs);
            }
        }
        // MediaStore (and general)
        else if ("content".equalsIgnoreCase(uri.getScheme())) {
            return getDataColumn(context, uri, null, null);
        }
        // File
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }
        return null;
    }

    public String getDataColumn(Context context, Uri uri, String selection, String[] selectionArgs) {
        Cursor cursor = null;
        final String column = "_data";
        final String[] projection = {column};

        try {
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs,
                    null);
            if (cursor != null && cursor.moveToFirst()) {
                final int column_index = cursor.getColumnIndexOrThrow(column);
                return cursor.getString(column_index);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }

    public boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    public boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    public boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (data == null) {
            return;
        }
        Uri uri = data.getData();//得到uri，后面就是将uri转化成file的过程。

        filePath = getPath(this, uri);

        //   RootUtil.runShell("/system/bin/mkdir "+dataDir+"/databases/", true);
        //   RootUtil.runShell("/system/bin/mkdir "+dataDir+"/shared_prefs/", true);
        RootUtil.runShell("/system/bin/mkdir " + "/storage/emulated/0/cats_set_UnZip/", true);

        Toasty.success(mContext, "导入中..", Toast.LENGTH_SHORT).show();
        RootUtil.runShell("/system/bin/mkdir /storage/emulated/0/cats_set_UnZip/", true);
        try {
            Toasty.success(mContext, "解析中..", Toast.LENGTH_SHORT).show();
            ZipUtil.UnZipFolder(filePath, "/storage/emulated/0/cats_set_UnZip/");
            //RootUtil.runShell("/system/bin/cp -f " + "/storage/emulated/0/cats_set_UnZip/cats_set/config.db" + " " + dataDir + "/databases/config.db", true);
            //RootUtil.runShell("/system/bin/cp -f " + "/storage/emulated/0/cats_set_UnZip/cats_set/imgui_config" + " " + dataDir + "/databases/imgui_config", true);
            //RootUtil.runShell("/system/bin/cp -f " + "/storage/emulated/0/cats_set_UnZip/cats_set/config.xml" + " " + dataDir + "/shared_prefs/config.xml", true);
            RootUtil.runShell("/system/bin/cp -f " + "/storage/emulated/0/cats_set_UnZip/cats_set/imgui_config" + " " + dataDir , true);

            if (RootUtil.runShell("/system/bin/ls " + dataDir , true).contains("imgui_config")) {
                DialogFactory.createTipsDialog(SettingActivity.this, "温馨提示", "配置加载完毕重启软件生效!", true).show();

                Toasty.success(mContext, "配置加载完毕重启软件生效", Toast.LENGTH_SHORT).show();
                //   System.exit(0);

            } else {
                Toasty.error(mContext, "配置加载错误,请检测您导入的是否正确", Toast.LENGTH_SHORT).show();
            }
          //  RootUtil.runShell("/system/bin/chmod -R 0777 " + dataDir + "/databases", true);
         //   RootUtil.runShell("/system/bin/chmod -R 0777 " + dataDir + "/shared_prefs", true);
         //   if (RootUtil.runShell("/system/bin/ls " + dataDir + "/databases", true).contains("config") || RootUtil.runShell("/system/bin/ls " + dataDir + "/shared_prefs", true).contains("config")) {
         //       DialogFactory.createTipsDialog(SettingActivity.this, "温馨提示", "配置加载完毕重启软件生效!", true).show();

           //     Toasty.success(mContext, "配置加载完毕重启软件生效", Toast.LENGTH_SHORT).show();
                //   System.exit(0);

          //  } else {
          //      Toasty.error(mContext, "配置加载错误,请检测您导入的是否正确", Toast.LENGTH_SHORT).show();
          //  }
        } catch (Exception e) {
            Toasty.error(mContext, "发生异常", Toast.LENGTH_SHORT).show();
            e.printStackTrace();
        }
    }


    private void initView() throws PackageManager.NameNotFoundException {

        // Intent intent1 = new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
        // startActivity(intent1);

        Button export_set = findViewById(R.id.export_set);
        Button import_set = findViewById(R.id.import_set);

        dataDir = "/storage/emulated/0/imgui_init";//getPackageManager().getPackageInfo(getPackageName(), 0).applicationInfo.dataDir;

        export_set.setOnClickListener(v -> {
            RootUtil.runShell("/system/bin/rm -r /storage/emulated/0/cats_set", true);
            RootUtil.runShell("/system/bin/rm -r /storage/emulated/0/配置.zip", true);
            RootUtil.runShell("/system/bin/mkdir /storage/emulated/0/cats_set/", true);
            Toasty.success(mContext, "导出中..", Toast.LENGTH_SHORT).show();
           // RootUtil.runShell("/system/bin/cp -f " + dataDir + "/databases/config.db" + " " + "/storage/emulated/0/cats_set", true);
          //  RootUtil.runShell("/system/bin/cp -f " + dataDir + "/databases/imgui_config" + " " + "/storage/emulated/0/cats_set", true);
          //  RootUtil.runShell("/system/bin/cp -f " + dataDir + "/shared_prefs/config.xml" + " " + "/storage/emulated/0/cats_set", true);
            RootUtil.runShell("/system/bin/cp -f " + dataDir + "/imgui_config" + " "+"/storage/emulated/0/cats_set", true);

            try {
                ZipUtil.zip("/storage/emulated/0/cats_set", "/storage/emulated/0/配置.zip");
            } catch (IOException e) {
                e.printStackTrace();
            }
            Toasty.success(mContext, "导出完成:/storage/emulated/0/配置.zip", Toast.LENGTH_SHORT).show();
        });
        import_set.setOnClickListener(v -> {
            Toasty.success(mContext, "选择你导入的文件", Toast.LENGTH_SHORT).show();
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("*/*");
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            startActivityIfNeeded(intent, 1);

        });

        final CheckBox hidden = findViewById(R.id.hidden);
        hidden.setChecked(getConfig((String) hidden.getText()));


        hidden.setOnClickListener(v -> {
            Config.setHidden(hidden.isChecked());
            setValue(String.valueOf(hidden.getText()), hidden.isChecked());
            if (hidden.isChecked())
                Toasty.success(mContext, "开启", Toast.LENGTH_SHORT).show();
            else
                Toasty.success(mContext, "关闭", Toast.LENGTH_SHORT).show();
        });
        LinearLayout bt_back = findViewById(R.id.bt_back);

        bt_back.setOnClickListener(v -> {
            startActivity(new Intent(mContext, MainActivity.class));
            finish();
        });

    }

    private void setValue(String key, boolean b) {
        SharedPreferences sp = this.getSharedPreferences("config", Context.MODE_PRIVATE);
        SharedPreferences.Editor ed = sp.edit();
        ed.putBoolean(key, b);
        ed.apply();
    }

    boolean getConfig(String key) {
        SharedPreferences sp = this.getSharedPreferences("config", Context.MODE_PRIVATE);
        return sp.getBoolean(key, false);
        // return !key.equals("");
    }
}
