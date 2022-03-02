package me.simple.lib.exception;

import android.util.SparseArray;

public class ErrorCode {

    public static final int NULL_DATA = 0x100;
    public static final int NO_NET = 0x101;
    public static final int NO_DATA = 0x102;

    private static final SparseArray<String> errorMsgHashMap = new SparseArray<>();
    static {
        errorMsgHashMap.put(NULL_DATA, "服务器错误");
        errorMsgHashMap.put(NO_NET, "网络异常");
        errorMsgHashMap.put(NO_DATA, "数据为空");
    }

    public static String getErrorMsg(int code) {
        return errorMsgHashMap.get(code);
    }
}
