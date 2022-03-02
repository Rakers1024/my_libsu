package me.simple.lib.version.request;

import org.jetbrains.annotations.NotNull;

import java.util.List;

import me.simple.lib.base.BaseReponse;
import me.simple.lib.note.bean.NoticeBean;
import me.simple.lib.version.bean.VersionBean;


public class GetVersionReponse extends BaseReponse {
    public VersionBean info;
   // public List<VersionBean> info;
    @Override
    public @NotNull String toString() {
        return "GetVersionReponse{" +
                "ret='" + ret + '\'' +
                ", msg='" + msg + '\'' +
                ", info=[" + info + ']' +
                ", time='" + time + '\'' +
                '}';
    }
}


