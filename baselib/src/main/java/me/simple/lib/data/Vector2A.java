package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Vector2A implements Parcelable {
    private float X;
    private float Y;


    public float getX() {
        return X;
    }

    public float getY() {
        return Y;
    }

    public Vector2A() {
    }

    protected Vector2A(Parcel in) {
        X = in.readFloat();
        Y = in.readFloat();
    }

    public void setX(float x) {
        X = x;
    }

    public void setY(float y) {
        Y = y;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeFloat(X);
        dest.writeFloat(Y);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<Vector2A> CREATOR = new Creator<Vector2A>() {
        @Override
        public Vector2A createFromParcel(Parcel in) {
            return new Vector2A(in);
        }

        @Override
        public Vector2A[] newArray(int size) {
            return new Vector2A[size];
        }
    };

    @Override
    public String toString() {
        return "Vector2A{" +
                "X=" + X +
                ", Y=" + Y +
                '}';
    }
}
