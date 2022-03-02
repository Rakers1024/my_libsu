package me.simple.lib.pay.request;

import me.simple.lib.base.BaseRequest;
import me.simple.lib.utils.EncrypUtils;

public class UserRenewRequest extends BaseRequest {

    private String uin;
    private String kami;

    public UserRenewRequest() {
        super();
    }

    public void setUin(String uin1) {
        final String t = EncrypUtils.getTimeStamp(2);
        uin = EncrypUtils.oOOOooooOOoOOoo(uin1, t + "uoidufoisuvkjhdfksdh");
    }

    public void setPayCard(String value) {
        kami = value;
    }

    @Override
    protected String getAction() {
        return "UserRenew";
    }
}
