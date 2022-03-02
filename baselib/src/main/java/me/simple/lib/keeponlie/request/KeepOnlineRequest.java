package me.simple.lib.keeponlie.request;

import me.simple.lib.base.BaseRequest;

public class KeepOnlineRequest extends BaseRequest {

    private String token;

    public KeepOnlineRequest() {
        super();
    }

    public void setToken(String a){
        token = a;
    }

    @Override
    protected String getAction() {
        return "KeepOnline";
    }
}
