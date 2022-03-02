package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

import org.jetbrains.annotations.NotNull;

public class BoneData implements Parcelable {
    private Vector3A Head;
    private Vector3A Chest;
    private Vector3A Pelvis;
    private Vector3A Left_Shoulder;
    private Vector3A Right_Shoulder;
    private Vector3A Left_Elbow;
    private Vector3A Right_Elbow;
    private Vector3A Left_Wrist;
    private Vector3A Right_Wrist;
    private Vector3A Left_Thigh;
    private Vector3A Right_Thigh;
    private Vector3A Left_Knee;
    private Vector3A Right_Knee;
    private Vector3A Left_Ankle;
    private Vector3A Right_Ankle;


    public Vector3A getChest() {
        return Chest;
    }

    public Vector3A getHead() {
        return Head;
    }

    public Vector3A getLeft_Ankle() {
        return Left_Ankle;
    }

    public Vector3A getLeft_Elbow() {
        return Left_Elbow;
    }

    public Vector3A getLeft_Knee() {
        return Left_Knee;
    }

    public Vector3A getLeft_Shoulder() {
        return Left_Shoulder;
    }

    public Vector3A getLeft_Thigh() {
        return Left_Thigh;
    }

    public Vector3A getLeft_Wrist() {
        return Left_Wrist;
    }

    public Vector3A getPelvis() {
        return Pelvis;
    }

    public Vector3A getRight_Ankle() {
        return Right_Ankle;
    }

    public Vector3A getRight_Elbow() {
        return Right_Elbow;
    }

    public Vector3A getRight_Knee() {
        return Right_Knee;
    }

    public Vector3A getRight_Shoulder() {
        return Right_Shoulder;
    }

    public Vector3A getRight_Thigh() {
        return Right_Thigh;
    }

    public Vector3A getRight_Wrist() {
        return Right_Wrist;
    }

    public BoneData() {
    }

    protected BoneData(Parcel in) {
        Head = in.readParcelable(Vector3A.class.getClassLoader());
        Chest = in.readParcelable(Vector3A.class.getClassLoader());
        Pelvis = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Shoulder = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Shoulder = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Elbow = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Elbow = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Wrist = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Wrist = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Thigh = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Thigh = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Knee = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Knee = in.readParcelable(Vector3A.class.getClassLoader());
        Left_Ankle = in.readParcelable(Vector3A.class.getClassLoader());
        Right_Ankle = in.readParcelable(Vector3A.class.getClassLoader());

    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeParcelable(Head, flags);
        dest.writeParcelable(Chest, flags);
        dest.writeParcelable(Pelvis, flags);
        dest.writeParcelable(Left_Shoulder, flags);
        dest.writeParcelable(Right_Shoulder, flags);
        dest.writeParcelable(Left_Elbow, flags);
        dest.writeParcelable(Right_Elbow, flags);
        dest.writeParcelable(Left_Wrist, flags);
        dest.writeParcelable(Right_Wrist, flags);
        dest.writeParcelable(Left_Thigh, flags);
        dest.writeParcelable(Right_Thigh, flags);
        dest.writeParcelable(Left_Knee, flags);
        dest.writeParcelable(Right_Knee, flags);
        dest.writeParcelable(Left_Ankle, flags);
        dest.writeParcelable(Right_Ankle, flags);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<BoneData> CREATOR = new Creator<BoneData>() {
        @Override
        public BoneData createFromParcel(Parcel in) {
            return new BoneData(in);
        }

        @Override
        public BoneData[] newArray(int size) {
            return new BoneData[size];
        }
    };

    @Override
    public @NotNull String toString() {
        return "BoneData{" +
                "Head=" + Head +
                ", Chest=" + Chest +
                ", Pelvis=" + Pelvis +
                ", Left_Shoulder=" + Left_Shoulder +
                ", Right_Shoulder=" + Right_Shoulder +
                ", Left_Elbow=" + Left_Elbow +
                ", Right_Elbow=" + Right_Elbow +
                ", Left_Wrist=" + Left_Wrist +
                ", Right_Wrist=" + Right_Wrist +
                ", Left_Thigh=" + Left_Thigh +
                ", Right_Thigh=" + Right_Thigh +
                ", Left_Knee=" + Left_Knee +
                ", Right_Knee=" + Right_Knee +
                ", Left_Ankle=" + Left_Ankle +
                ", Right_Ankle=" + Right_Ankle +
                '}';
    }
}
