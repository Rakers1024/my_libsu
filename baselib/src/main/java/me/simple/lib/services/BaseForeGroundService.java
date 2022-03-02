package me.simple.lib.services;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;

import androidx.annotation.Nullable;

import me.simple.lib.utils.LogUtils;

public abstract class BaseForeGroundService extends Service {

    protected String TAG = getClass().getSimpleName();
    private Handler mainHandler;
    private Handler aSynHandler;


    @Override
    public void onCreate() {
        super.onCreate();
        mainHandler = new Handler(Looper.getMainLooper());
        HandlerThread handlerThread = new HandlerThread(TAG);
        handlerThread.start();
        aSynHandler = new Handler(handlerThread.getLooper());
        notifyForeGround(getId(), getName());
    }


    protected abstract String getId();

    protected abstract String getName();

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private static int idIndex = 0;
    private boolean isForeGround = false;

    private void notifyForeGround(String id, String name) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            //LogUtils.D(TAG, "startForeground");
            NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            NotificationChannel channel = new NotificationChannel(id, name, NotificationManager.IMPORTANCE_HIGH);
            manager.createNotificationChannel(channel);
            Notification notification = new Notification.Builder(this, id)
                    .build();
            startForeground(idIndex++, notification);
            isForeGround = true;
        }
    }

    public Handler getASynHandler() {
        return aSynHandler;
    }

    public Handler getMainHandler() {
        return mainHandler;
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        if (null != getASynHandler()) {
            getASynHandler().removeCallbacksAndMessages(null);
        }
        if (null != getMainHandler()) {
            getASynHandler().removeCallbacksAndMessages(null);
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && isForeGround) {
            stopForeground(true);
        }
    }

}
