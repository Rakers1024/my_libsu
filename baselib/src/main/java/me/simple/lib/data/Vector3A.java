package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Vector3A implements Parcelable {
    private float X;
    private float Y;
    private float Z;


    public float getX() {
        return X;
    }

    public float getY() {
        return Y;
    }

    public float getZ() {
        return Z;
    }

    public Vector3A() {
    }

    protected Vector3A(Parcel in) {
        X = in.readFloat();
        Y = in.readFloat();
        Z = in.readFloat();
    }

    public void setX(float x) {
        X = x;
    }

    public void setY(float y) {
        Y = y;
    }

    public void setZ(float z) {
        Z = z;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeFloat(X);
        dest.writeFloat(Y);
        dest.writeFloat(Z);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<Vector3A> CREATOR = new Creator<Vector3A>() {
        @Override
        public Vector3A createFromParcel(Parcel in) {
            return new Vector3A(in);
        }

        @Override
        public Vector3A[] newArray(int size) {
            return new Vector3A[size];
        }
    };

    @Override
    public String toString() {
        return "Vector3A{" +
                "X=" + X +
                ", Y=" + Y +
                ", Z=" + Z +
                '}';
    }
}
