package me.simple.lib.login.request;

import me.simple.lib.base.BaseReponse;

public class LoginUserReponse extends BaseReponse {
    public String token;
    public String pic;
    public String vip;
    public String name;
    public String isCharge;

    @Override
    public String toString() {
        return "LoginUserReponse{" +
                "token='" + token + '\'' +
                ", pic='" + pic + '\'' +
                ", vip='" + vip + '\'' +
                ", name='" + name + '\'' +
                ", ret='" + ret + '\'' +
                ", msg='" + msg + '\'' +
                ", time='" + time + '\'' +
                ", isCharge='" + isCharge + '\'' +
                '}';
    }
}
