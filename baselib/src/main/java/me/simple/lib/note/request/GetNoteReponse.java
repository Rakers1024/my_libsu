package me.simple.lib.note.request;

import java.util.List;

import me.simple.lib.base.BaseReponse;
import me.simple.lib.note.bean.NoticeBean;

public class GetNoteReponse extends BaseReponse {
    public List<NoticeBean> note;
    public String isCharge;
    @Override
    public String toString() {
        return "GetNoteReponse{" +
                "ret='" + ret + '\'' +
                ", msg='" + msg + '\'' +
                ", note=[" + note + ']' +
                ", time='" + time + '\'' +
                ", isCharge='" + isCharge + '\'' +
                '}';
    }
}
