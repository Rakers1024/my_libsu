package me.simple.lib.activity;

import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.util.TypedValue;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.Lifecycle;
import androidx.lifecycle.LifecycleOwner;

import me.simple.lib.BuildConfig;
import me.simple.lib.utils.ActivityManger;
import me.simple.lib.utils.ImmersionBar;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ReflectionApiCheck;

public class BaseActivity extends AppCompatActivity {

    public Context mContext;
    protected String TAG = getClass().getSimpleName();

    private Handler mainHandler;
    private Handler aSynHandler;



    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LogUtils.D(TAG, "onCreate");
        mainHandler = new Handler(Looper.getMainLooper());
        ActivityManger.addActivity(this);
        ImmersionBar.init(this);
        mContext = this;
        //过魅族权限;
        ReflectionApiCheck.exemptAll();
    }

    @Override
    protected void onStart() {
        super.onStart();
        LogUtils.D(TAG, "onStart");

    }

    @Override
    protected void onStop() {
        super.onStop();
        LogUtils.D(TAG, "onStop");
    }

    @Override
    protected void onResume() {
        super.onResume();
        LogUtils.D(TAG, "onResume");
    }

    @Override
    protected void onDestroy() {
        LogUtils.D(TAG, "onDestroy");
        super.onDestroy();
        if (null != mainHandler) {
            mainHandler.removeCallbacksAndMessages(null);
        }
        if (null != aSynHandler) {
            aSynHandler.removeCallbacksAndMessages(null);
        }
        ActivityManger.removeActivity(this);
    }

    public Handler getASynHandler() {
        if (null == aSynHandler) {
            HandlerThread handlerThread = new HandlerThread(TAG);
            handlerThread.start();
            aSynHandler = new Handler(handlerThread.getLooper());
        }
        return aSynHandler;
    }

    public Handler getMainHandler() {
        return mainHandler;
    }

    public LifecycleOwner getLifeOwner() {
        return this;
    }

    public int dp2px(int dpVal) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dpVal, getResources().getDisplayMetrics());
    }
}
