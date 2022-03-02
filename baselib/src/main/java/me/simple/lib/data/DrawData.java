package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.Arrays;

public class DrawData implements Parcelable {

    private boolean Success; //状态
    private boolean isHall; //大厅
    private int PlayerCount; //人
    private int GrenadeCount; //投掷物
    private int VehicleCount; //车
    private int ItemsCount; //物资
    private int AirBoxCount; //空投盒子飞机
    private float fov ;
    private boolean openAccumulation;
    private PlayerData[] Players;
    private ItemData[] Grenades;
    private ItemData[] Vehicle;
    private ItemData[] Items;
    private ItemData[] AirBox;
    private String testTmpData;
    private int isadb;
    protected DrawData(Parcel in) {
        Success = in.readByte() != 0;
        isHall = in.readByte() != 0;
        PlayerCount = in.readInt();
        GrenadeCount = in.readInt();
        VehicleCount = in.readInt();
        ItemsCount = in.readInt();
        AirBoxCount = in.readInt();
        fov = in.readFloat();
        openAccumulation = in.readByte() != 0;
        Players = in.createTypedArray(PlayerData.CREATOR);
        Grenades = in.createTypedArray(ItemData.CREATOR);
        Vehicle = in.createTypedArray(ItemData.CREATOR);
        Items = in.createTypedArray(ItemData.CREATOR);
        AirBox = in.createTypedArray(ItemData.CREATOR);
        testTmpData = in.readString();
        isadb = in.readInt();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeByte((byte) (Success ? 1 : 0));
        dest.writeByte((byte) (isHall ? 1 : 0));
        dest.writeInt(PlayerCount);
        dest.writeInt(GrenadeCount);
        dest.writeInt(VehicleCount);
        dest.writeInt(ItemsCount);
        dest.writeInt(AirBoxCount);
        dest.writeFloat(fov);
        dest.writeByte((byte) (openAccumulation ? 1 : 0));
        dest.writeTypedArray(Players, flags);
        dest.writeTypedArray(Grenades, flags);
        dest.writeTypedArray(Vehicle, flags);
        dest.writeTypedArray(Items, flags);
        dest.writeTypedArray(AirBox, flags);
        dest.writeString(testTmpData);
        dest.writeInt(isadb);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<DrawData> CREATOR = new Creator<DrawData>() {
        @Override
        public DrawData createFromParcel(Parcel in) {
            return new DrawData(in);
        }

        @Override
        public DrawData[] newArray(int size) {
            return new DrawData[size];
        }
    };

    public boolean isSuccess() {
        return Success;
    }

    public boolean isHall() {
        return isHall;
    }

    public int getPlayerCount() {
        return PlayerCount;
    }

    public int getGrenadeCount() {
        return GrenadeCount;
    }

    public int getVehicleCount() {
        return VehicleCount;
    }

    public int getItemsCount() {
        return ItemsCount;
    }

    public int getAirBoxCount() {
        return AirBoxCount;
    }

    public PlayerData[] getPlayers() {
        return Players;
    }

    public ItemData[] getGrenades() {
        return Grenades;
    }

    public ItemData[] getVehicle() {
        return Vehicle;
    }

    public ItemData[] getItems() {
        return Items;
    }

    public ItemData[] getAirBox() {
        return AirBox;
    }

    public float getFov() {
        return fov;
    }
    public void setFov(float FOV) {
         fov = FOV;
    }
    public boolean getOpenAccumulation() {
        return openAccumulation;
    }

    public String getTestTmpData() {
        return testTmpData;
    }

    public int getAdb() {
        return isadb;
    }

    public DrawData() {

    }

    @Override
    public String toString() {
        return "DrawData{" +
                "Success=" + Success +
                ", isHall=" + isHall +
                ", PlayerCount=" + PlayerCount +
                ", GrenadeCount=" + GrenadeCount +
                ", VehicleCount=" + VehicleCount +
                ", ItemsCount=" + ItemsCount +
                ", AirBoxCount=" + AirBoxCount +
                ", fov="+fov+
                ", openAccumulation="+openAccumulation+
                ", Players=" + Arrays.toString(Players) +
                ", Grenades=" + Arrays.toString(Grenades) +
                ", Vehicle=" + Arrays.toString(Vehicle) +
                ", Items=" + Arrays.toString(Items) +
                ", AirBox=" + Arrays.toString(AirBox) +
                ", testTmpData="+testTmpData+ '\'' +
                ", isAdb="+isadb+

                '}';
    }
}
