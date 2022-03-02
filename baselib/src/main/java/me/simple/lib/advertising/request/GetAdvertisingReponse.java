package me.simple.lib.advertising.request;

import me.simple.lib.advertising.bean.AdvertisingBean;
import me.simple.lib.base.BaseReponse;


public class GetAdvertisingReponse extends BaseReponse {
    public AdvertisingBean info;

    @Override
    public String toString() {
        return "GetAdvertisingReponse{" +
                "ret='" + ret + '\'' +
                ", msg='" + msg + '\'' +
                ", info=[" + info + ']' +
                ", time='" + time + '\'' +
                '}';
    }
}
