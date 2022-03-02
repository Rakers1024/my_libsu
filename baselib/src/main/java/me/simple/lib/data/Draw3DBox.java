package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

public class Draw3DBox implements Parcelable {
    Vector2A v1;
    Vector2A v2;
    Vector2A v3;
    Vector2A v4;
    Vector2A v5;
    Vector2A v6;
    Vector2A v7;
    Vector2A v8;


    public Vector2A getV1() {
        return v1;
    }

    public Vector2A getV2() {
        return v2;
    }

    public Vector2A getV3() {
        return v3;
    }

    public Vector2A getV4() {
        return v4;
    }

    public Vector2A getV5() {
        return v5;
    }

    public Vector2A getV6() {
        return v6;
    }

    public Vector2A getV7() {
        return v7;
    }

    public Vector2A getV8() {
        return v8;
    }

    public Draw3DBox() {
    }

    protected Draw3DBox(Parcel in) {
        v1 = in.readParcelable(Vector2A.class.getClassLoader());
        v2 = in.readParcelable(Vector2A.class.getClassLoader());
        v3 = in.readParcelable(Vector2A.class.getClassLoader());
        v4 = in.readParcelable(Vector2A.class.getClassLoader());
        v5 = in.readParcelable(Vector2A.class.getClassLoader());
        v6 = in.readParcelable(Vector2A.class.getClassLoader());
        v7 = in.readParcelable(Vector2A.class.getClassLoader());
        v8 = in.readParcelable(Vector2A.class.getClassLoader());
    }



    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeParcelable(v1, flags);
        dest.writeParcelable(v2, flags);
        dest.writeParcelable(v3, flags);
        dest.writeParcelable(v4, flags);
        dest.writeParcelable(v5, flags);
        dest.writeParcelable(v6, flags);
        dest.writeParcelable(v7, flags);
        dest.writeParcelable(v8, flags);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Parcelable.Creator<Draw3DBox> CREATOR = new Parcelable.Creator<Draw3DBox>() {
        @Override
        public Draw3DBox createFromParcel(Parcel in) {
            return new Draw3DBox(in);
        }

        @Override
        public Draw3DBox[] newArray(int size) {
            return new Draw3DBox[size];
        }
    };

    @Override
    public String toString() {
        return "Draw3DBox{" +
                "v1=" + v1 +
                ", v2=" + v2 +
                ", v3=" + v3 +
                ", v4=" + v4 +
                ", v5=" + v5 +
                ", v6=" + v6 +
                ", v7=" + v7 +
                ", v8=" + v8 +
                '}';
    }
}
