package me.simple.lib.net;

import com.google.gson.Gson;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import me.simple.lib.utils.LogUtils;

public class BaseRequestWraper<T> {

    T data;

    public BaseRequestWraper(T realData) {
        this.data = realData;
    }

    public Map<String, String> getMap() {
        Map<String, String> map = new HashMap<>();
   //     LogUtils.D("getMap","getMap" + "new Gson().toJson(data) : " + new Gson().toJson(data));
//        map.put("data", EncryptUtil.encrypt(new Gson().toJson(data)));
        Gson gson = new Gson();
        JsonElement jsonElement = gson.toJsonTree(data);
        if (null != jsonElement) {
            JsonObject jsonObject = jsonElement.getAsJsonObject();
            Set<String> strings = jsonObject.keySet();
            for (String s : strings) {
              //  LogUtils.D("getMap", s + " = " + jsonObject.get(s).getAsString());
                map.put(s, jsonObject.get(s).getAsString());
            }
        }
        return map;
    }
}
