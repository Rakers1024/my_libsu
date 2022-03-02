package com.kmxs.reader.utils;

public class AppConfigManger {

    private static volatile AppConfigManger instance = null;
    private AppConfig mAppConfig;

    public AppConfigManger(){
        mAppConfig = new AppConfig();
    }

    public static AppConfigManger getInstance() {
        if (instance == null) {
            synchronized (AppConfigManger.class) {
                if (instance == null) {
                    instance = new AppConfigManger();
                }
            }
        }
        return instance;
    }

    public AppConfig readAppConfig() {
        return mAppConfig;
    }

}
