package me.simple.lib.login.request;

import me.simple.lib.base.BaseRequest;
import me.simple.lib.utils.EncrypUtils;
import me.simple.lib.utils.SafeUtils;

public class LoginUserRequest extends BaseRequest {

    private String uin;
    private String markcode;
    private String pwd;

    public LoginUserRequest() {
        super();
    }

    public void setUin(String uin1) {
        final String t = EncrypUtils.getTimeStamp(2);
        uin = EncrypUtils.oOOOooooOOoOOoo(uin1, t + "uoidufoisuvkjhdfksdh");
    }

    public void setMarkCode(String deviceId) {
        markcode = EncrypUtils.oOOOooooOOoOOoo(deviceId, SafeUtils.rc4key);
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    @Override
    protected String getAction() {
        return "UserLogin";
    }
}