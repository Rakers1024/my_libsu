package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;


public class PlayerData implements Parcelable {
    private boolean isAI;
    private boolean isRat;
    private int TeamID;
    private float Health;
    private float Angle;
    private int State;
    private boolean isVisibility;
    private float Distance;
    private String PlayerName;
    private float HeadSize;
    private BoneData mBoneData;
    private Vector4A Location;
    private Vector4A runningLoc;

    private Vector2A RadarLocation;
    private WeaponData Weapon;
    private  Vector2A RotationAngle;
    private int aimD2D;
    private Draw3DBox draw3DBox;

    public PlayerData() {

    }

    protected PlayerData(Parcel in) {
        isAI = in.readByte() != 0;
        isRat = in.readByte() != 0;
        TeamID = in.readInt();
        Health = in.readFloat();
        Angle = in.readFloat();
        State = in.readInt();
        isVisibility = in.readByte() != 0;
        Distance = in.readFloat();
        PlayerName = in.readString();
        HeadSize = in.readFloat();
        mBoneData = in.readParcelable(BoneData.class.getClassLoader());
        Location = in.readParcelable(Vector4A.class.getClassLoader());
        RadarLocation = in.readParcelable(Vector2A.class.getClassLoader());
        Weapon = in.readParcelable(WeaponData.class.getClassLoader());
        RotationAngle = in.readParcelable(Vector2A.class.getClassLoader());
        aimD2D = in.readInt();
        runningLoc = in.readParcelable(Vector4A.class.getClassLoader());
        draw3DBox = in.readParcelable(Draw3DBox.class.getClassLoader());


    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeByte((byte) (isAI ? 1 : 0));
        dest.writeByte((byte) (isRat ? 1 : 0));
        dest.writeInt(TeamID);
        dest.writeFloat(Health);
        dest.writeFloat(Angle);
        dest.writeInt(State);
        dest.writeByte((byte) (isVisibility ? 1 : 0));
        dest.writeFloat(Distance);
        dest.writeString(PlayerName);
        dest.writeFloat(HeadSize);
        dest.writeParcelable(mBoneData, flags);
        dest.writeParcelable(Location, flags);
        dest.writeParcelable(RadarLocation, flags);
        dest.writeParcelable(Weapon,flags);
        dest.writeParcelable(RotationAngle,flags);
        dest.writeInt(aimD2D);
        dest.writeParcelable(runningLoc, flags);
        dest.writeParcelable(draw3DBox, flags);

    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<PlayerData> CREATOR = new Creator<PlayerData>() {
        @Override
        public PlayerData createFromParcel(Parcel in) {
            return new PlayerData(in);
        }

        @Override
        public PlayerData[] newArray(int size) {
            return new PlayerData[size];
        }
    };

    public int getAimD2D(){
        return aimD2D;
    }
    public float getHeadSize() {
        return HeadSize;
    }

    public boolean getisAI() {
        return isAI;
    }

    public boolean getisRat() {
        return isRat;
    }

    public int getTeamID() {
        return TeamID;
    }

    public float getHealth() {
        return Health;
    }

    public float getAngle() {
        return Angle;
    }

    public int getState() {
        return State;
    }

    public boolean getVisibility() {
        return isVisibility;
    }

    public float getDistance() {
        return Distance;
    }

    public String getPlayerName() {
        return PlayerName;
    }

    public BoneData getBoneData() {
        return mBoneData;
    }

    public Vector4A getLocation() {
        return Location;
    }

    public Vector4A getRunningLoc() {
        return runningLoc;
    }

    public Vector2A getRadarLocation() {
        return RadarLocation;
    }


    public WeaponData getWeapon() {
        return Weapon;
    }

    public Vector2A getRadarRotationAngle() {
        return RotationAngle;
    }

    public Draw3DBox getDraw3DBox() {
        return draw3DBox;
    }

    @Override
    public String toString() {
        return "PlayerData{" +
                "isAI=" + isAI +
                ", isRat=" + isRat +
                ", TeamID=" + TeamID +
                ", Health=" + Health +
                ", Angle=" + Angle +
                ", State=" + State +
                ", isVisibility=" + isVisibility +
                ", Distance=" + Distance +
                ", PlayerName='" + PlayerName + '\'' +
                ", HeadSize=" + HeadSize +
                ", mBoneData=" + mBoneData +
                ", Location=" + Location +
                ", RadarLocation=" + RadarLocation +
                ", Weapon=" + Weapon +
                ", RotationAngle="+RotationAngle+
                ", aimD2D="+aimD2D+
                ", runningLoc="+runningLoc+
                ", draw3DBox="+draw3DBox+
                '}';
    }
}
