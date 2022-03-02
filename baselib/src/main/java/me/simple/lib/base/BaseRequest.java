package me.simple.lib.base;

import me.simple.lib.utils.EncrypUtils;
import me.simple.lib.utils.SafeUtils;

public abstract class BaseRequest {

    private String appid;
    private String act;
    private String t;

    protected BaseRequest() {
        act = getAction();
        appid = SafeUtils.appid;
        t = EncrypUtils.getTimeStamp(2);
    }

    protected abstract String getAction();
}
