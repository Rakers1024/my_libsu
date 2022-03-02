package com.kmxs.reader.core;

import android.os.Parcel;
import android.os.Parcelable;

public class Config implements Parcelable {
    public static final String BROADCAST_KEY = "DrawPaintConfig";


    public int   mLeftOffset, mUpOffset;
    public float mProportion;
    public float[] mMap = new float[2];

    private static boolean isHidden;//隐藏后台
    private static boolean isRecordScreen; //直播录屏
    public static void setHidden(boolean data){
        isHidden = data;
    }
    public static boolean getHidden(){
        return isHidden;
    }
    public static void setRecordScreen(boolean data){
        isRecordScreen = data;
    }
    public static boolean getRecordScreen(){
        return isRecordScreen;
    }
    public static boolean isModuleInstall = false;
    public static boolean isModuleUpdate = false;
    public Config() {

    }


    protected Config(Parcel in) {

        mLeftOffset = in.readInt();
        mProportion = in.readFloat();
        mMap = in.createFloatArray();

    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mLeftOffset);
        dest.writeInt(mUpOffset);
        dest.writeFloat(mProportion);
        dest.writeFloatArray(mMap);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<Config> CREATOR = new Creator<Config>() {
        @Override
        public Config createFromParcel(Parcel in) {
            return new Config(in);
        }

        @Override
        public Config[] newArray(int size) {
            return new Config[size];
        }
    };

    public void setmUpOffset(int mUpOffset) {
        this.mUpOffset = mUpOffset;
    }

    public void setMap(float[] mMap) {
        this.mMap = mMap;
    }

    public void setmLeftOffset(int mLeftOffset) {
        this.mLeftOffset = mLeftOffset;
    }

    public void setmProportion(float mProportion) {
        this.mProportion = mProportion;
    }




}
