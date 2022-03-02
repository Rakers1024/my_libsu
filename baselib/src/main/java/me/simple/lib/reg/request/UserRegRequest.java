package me.simple.lib.reg.request;

import me.simple.lib.base.BaseRequest;
import me.simple.lib.utils.EncrypUtils;

public class UserRegRequest extends BaseRequest {

    private String uin;
    private String markcode;
    private String email;
    private String pwd;

    public UserRegRequest() {
        super();
    }

    public void setUin(String uin1) {
        final String t = EncrypUtils.getTimeStamp(2);
        uin = EncrypUtils.oOOOooooOOoOOoo(uin1, t + "uoidufoisuvkjhdfksdh");
    }

    public void setMarkCode(String value) {
        markcode = value;
    }

    public void setEmail(String value) {
        email = value;
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }

    @Override
    protected String getAction() {
        return "UserReg";
    }
}
