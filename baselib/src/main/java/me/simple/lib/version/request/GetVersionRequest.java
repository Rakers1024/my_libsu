package me.simple.lib.version.request;

import me.simple.lib.base.BaseRequest;

public class GetVersionRequest extends BaseRequest {

    public GetVersionRequest() {
        super();
    }

    @Override
    protected String getAction() {
        return "getVersionUpdate";
    }
}
