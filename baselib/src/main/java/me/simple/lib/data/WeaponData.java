package me.simple.lib.data;

import android.os.Build;
import android.os.Parcel;
import android.os.Parcelable;

import androidx.annotation.RequiresApi;

import java.util.Arrays;

public class WeaponData implements Parcelable {
    private boolean isWeapon;
    private int id;
    private int ammo;
    private int clip;

    public int getWeaponID() {
        return id;
    }
    public int getWeaponAmmo() {
        return ammo;
    }
    public int getWeaponClip() { return clip; }
    public boolean getWeaponIsWeapon() {
        return isWeapon;
    }


    public WeaponData() {

    }

    protected WeaponData(Parcel in) {
        isWeapon = in.readByte() != 0;
        id = in.readInt();
        ammo = in.readInt();
        clip = in.readInt();
    }
    @RequiresApi(api = Build.VERSION_CODES.Q)
    @Override
    public void writeToParcel(Parcel dest, int flags) {
       dest.writeByte((byte) (isWeapon ? 1 : 0));
        dest.writeInt(id);
        dest.writeInt(ammo);
        dest.writeInt(clip);
    }

    public static final Creator<WeaponData> CREATOR = new Creator<WeaponData>() {
        @RequiresApi(api = Build.VERSION_CODES.Q)
        @Override
        public WeaponData createFromParcel(Parcel in) {
            return new WeaponData(in);
        }

        @Override
        public WeaponData[] newArray(int size) {
            return new WeaponData[size];
        }
    };



    @Override
    public int describeContents() {
        return 0;
    }



    @Override
    public String toString() {
        return "WeaponData{" +
                "isWeapon=" + isWeapon +
                ", id=" + id +
                ", ammo=" + ammo +
                ", clip=" + clip+
                '}';
    }

}
