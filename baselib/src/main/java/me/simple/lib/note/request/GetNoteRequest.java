package me.simple.lib.note.request;

import me.simple.lib.base.BaseRequest;

public class GetNoteRequest extends BaseRequest {
    public GetNoteRequest() {
        super();
    }

    @Override
    protected String getAction() {
        return "getNotice";
    }
}
