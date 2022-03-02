package me.simple.lib.advertising.request;

import me.simple.lib.base.BaseRequest;

public class GetAdvertisingRequest extends BaseRequest {

    public GetAdvertisingRequest() {
        super();
    }

    @Override
    protected String getAction() {
        return "getAdvertising";
    }
}
