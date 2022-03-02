package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Vector4A implements Parcelable {
    private float x;
    private float y;
    private float h;
    private float w;

    public float getY() {
        return y;
    }

    public float getX() {
        return x;
    }

    public float getW() {
        return w;
    }

    public float getH() {
        return h;
    }

    public Vector4A() {
    }

    protected Vector4A(Parcel in) {
        x = in.readFloat();
        y = in.readFloat();
        h = in.readFloat();
        w = in.readFloat();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeFloat(x);
        dest.writeFloat(y);
        dest.writeFloat(h);
        dest.writeFloat(w);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<Vector4A> CREATOR = new Creator<Vector4A>() {
        @Override
        public Vector4A createFromParcel(Parcel in) {
            return new Vector4A(in);
        }

        @Override
        public Vector4A[] newArray(int size) {
            return new Vector4A[size];
        }
    };

    @Override
    public String toString() {
        return "Vector4A{" +
                "x=" + x +
                ", y=" + y +
                ", h=" + h +
                ", w=" + w +
                '}';
    }
}

