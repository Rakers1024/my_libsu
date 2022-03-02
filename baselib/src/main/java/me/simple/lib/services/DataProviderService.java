package me.simple.lib.services;

import android.content.Intent;
import android.os.IBinder;

import androidx.annotation.NonNull;

import com.topjohnwu.superuser.ipc.RootService;

import java.io.File;

import me.simple.lib.jni.DataProvider;
import me.simple.lib.utils.LogUtils;

public class DataProviderService extends RootService {
    @Override
    public void onCreate() {
        super.onCreate();
        File destFile2 = new File(getApplicationInfo().nativeLibraryDir, "libgoogle_set2.so");
        File destFile = new File(getApplicationInfo().nativeLibraryDir, "libgoogle_set_init.so");

        DataProvider.init(destFile.getAbsolutePath(),destFile2.getAbsolutePath());
        LogUtils.D("GameInject","绑定so");

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }


    @Override
    public IBinder onBind(@NonNull Intent intent) {
        return new RealDataProvider();
    }


}

