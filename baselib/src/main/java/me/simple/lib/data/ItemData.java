package me.simple.lib.data;

import android.os.Parcel;
import android.os.Parcelable;

import java.util.Arrays;

public class ItemData implements Parcelable {
    private float Distance;
    private String ItemName;
    private Vector3A Location;
    private int[] ItemID;
    private int ItemNum;
    private float ItemHP;
    private float ItemFuel;


    public float getDistance() {
        return Distance;
    }

    public String getItemName() {
        return ItemName;
    }

    public Vector3A getLocation() {
        return Location;
    }

    public int[] getItemID() {
        return ItemID;
    }

    public int getItemNum() {
        return ItemNum;
    }
    public float getItemHP() {
        return ItemHP;
    }
    public float getItemFuel() {
        return ItemFuel;
    }

    public ItemData() {
    }

    protected ItemData(Parcel in) {
        Distance = in.readFloat();
        ItemName = in.readString();
        Location = in.readParcelable(Vector3A.class.getClassLoader());
        ItemID = in.createIntArray();
        ItemNum = in.readInt();
        ItemHP = in.readFloat();
        ItemFuel = in.readFloat();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeFloat(Distance);
        dest.writeString(ItemName);
        dest.writeParcelable(Location, flags);
        dest.writeIntArray(ItemID);
        dest.writeInt(ItemNum);
        dest.writeFloat(ItemHP);
        dest.writeFloat(ItemFuel);

    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<ItemData> CREATOR = new Creator<ItemData>() {
        @Override
        public ItemData createFromParcel(Parcel in) {
            return new ItemData(in);
        }

        @Override
        public ItemData[] newArray(int size) {
            return new ItemData[size];
        }
    };

    @Override
    public String toString() {
        return "ItemData{" +
                "Distance=" + Distance +
                ", ItemName='" + ItemName + '\'' +
                ", Location=" + Location +
                ", ItemID=" + Arrays.toString(ItemID) +
                ", ItemNum=" + ItemNum +
                ", ItemHP ="+ItemHP+
                ", ItemFuel="+ItemFuel+
                '}';
    }
}
