package com.kmxs.reader.utils;

import android.os.Parcel;
import android.os.Parcelable;

public class AppConfig implements Parcelable {
    public String mToken = null;
    public String mHeader = null;
    public String mName = null;
    public String user = null;
    public String mTime = null;
    public String mVersion = "1.0";
    public String HOST = "http://183.131.79.130:5455";
    public String appid = "10000";
    public String rc4key = "fdgjdsfklbjlhksdjgpwajibsdb";
    public static boolean isSetMap;
    public boolean isAbnormal = false;
    public boolean isVPN = false;
    public int mVersionMode = 4;

    public AppConfig(){

    }

    protected AppConfig(Parcel in) {
        mToken = in.readString();
        mHeader = in.readString();
        mName = in.readString();
        mTime = in.readString();
        mVersion = in.readString();
        HOST = in.readString();
        appid = in.readString();
        rc4key = in.readString();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mToken);
        dest.writeString(mHeader);
        dest.writeString(mName);
        dest.writeString(mTime);
        dest.writeString(mVersion);
        dest.writeString(HOST);
        dest.writeString(appid);
        dest.writeString(rc4key);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<AppConfig> CREATOR = new Creator<AppConfig>() {
        @Override
        public AppConfig createFromParcel(Parcel in) {
            return new AppConfig(in);
        }

        @Override
        public AppConfig[] newArray(int size) {
            return new AppConfig[size];
        }
    };

    public String getAppid() {
        return appid;
    }

    public String getHost() {
        return HOST;
    }

    public String getHeader() {
        return mHeader;
    }

    public String getName() {
        return mName;
    }

    public String getTime() {
        return mTime;
    }

    public String getToken() {
        return mToken;
    }

    public String getVersion() {
        return mVersion;
    }
}
