package me.simple.lib.widget;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;

import java.io.IOException;

import me.simple.lib.utils.CustomDialog;
import me.simple.lib.utils.ShellUtils;

public class DialogFactory  {
    private final static String DEFAULT_TIPS_TITLE = "温馨提示";
    private final static String DEFAULT_OK_TEXT = "确定";

    public static CustomDialog createTipsDialog(Context context, String msg) {
        return  createTipsDialog(context, DEFAULT_TIPS_TITLE, msg, DEFAULT_OK_TEXT, false);
    }

    public static CustomDialog createTipsDialog(Context context, String msg, boolean isFinish) {
        return  createTipsDialog(context, DEFAULT_TIPS_TITLE, msg, DEFAULT_OK_TEXT, isFinish);
    }

    public static CustomDialog createTipsDialog(Context context, String title, String msg, boolean autoFinishActivity) {
        CustomDialog dialog = new CustomDialog(context);
        dialog.setTitle(title);
        dialog.setMessage(msg);
        dialog.setCanceledOnTouchOutside(false);
        dialog.setCancelable(false);
        dialog.setButton(DialogInterface.BUTTON_POSITIVE, DEFAULT_OK_TEXT, (thisDialog, which) -> {
            thisDialog.dismiss();
            if (autoFinishActivity && context instanceof Activity) {
                System.exit(0);
                ((Activity) context).finish();
            }
        });
        return dialog;
    }

    public static void createTipsDialogInstall(Context mContext, String t, String s, String name) {
        AlertDialog.Builder dialog = new AlertDialog.Builder(mContext);
        dialog.setCancelable(false);
        dialog.setMessage(s);
        dialog.setTitle(t);

        dialog.setNeutralButton("退出", (dialog13, which) -> {
            System.exit(0);
        });

        dialog.setPositiveButton("安装", (dialog1, which) -> {
            try {
                Runtime.getRuntime().exec("su -c pm install " + name);
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.exit(0);

        });
        dialog.show();
    }



    public static CustomDialog createTipsDialog(Context context, String title, String msg, String okText, boolean autoFinishActivity) {
        CustomDialog dialog = new CustomDialog(context);
        dialog.setTitle(title);
        dialog.setMessage(msg);
        dialog.setCanceledOnTouchOutside(false);
        dialog.setCancelable(false);
        dialog.setButton(DialogInterface.BUTTON_POSITIVE, okText, (thisDialog, which) -> {
            thisDialog.dismiss();
            if (autoFinishActivity && context instanceof Activity) {
                ((Activity) context).finish();
            }
        });
        return dialog;
    }

}
