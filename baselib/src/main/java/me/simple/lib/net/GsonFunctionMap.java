package me.simple.lib.net;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonNull;
import com.google.gson.JsonObject;

import java.lang.reflect.Type;

import io.reactivex.functions.Function;
import me.simple.lib.exception.NotClassException;
import me.simple.lib.exception.ResponseError;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ReflectUtils;
import me.simple.lib.utils.SafeUtils;

import static me.simple.lib.exception.ErrorCode.NULL_DATA;

public class GsonFunctionMap<T, R> implements Function<T, R> {
    private static final String TAG = "GsonFunctionMap";
    private Type mType;
    private GsonBuilder gsonBuilder;
    private int selectIndex = 0;
    private int wrapLevel = 0;
    private boolean isMainClass = false;
    private String originTimeStamp;

    public GsonFunctionMap(String t, Class returnClass) {
        getType(returnClass);
       // LogUtils.D(TAG, "mType: " + mType);
        originTimeStamp = t;
    }

    public GsonFunctionMap(Class returnClass) {
        getType(returnClass);
      //  LogUtils.D(TAG, "mType: " + mType);
    }

    public GsonFunctionMap(Class returnClass, boolean isMainClass) {
        this.isMainClass = isMainClass;
        getType(returnClass);
       // LogUtils.D(TAG, "mType: " + mType);
    }

    public GsonFunctionMap(Class returnClass, int wrapLevel, int selectIndex, boolean isMainClass) {
        this.selectIndex = selectIndex;
        this.wrapLevel = wrapLevel;
        this.isMainClass = isMainClass;
        getType(returnClass);
     //   LogUtils.D(TAG, "mType: " + mType);
    }

    public GsonFunctionMap(Class returnClass, GsonBuilder gsonBuilder) {
        this(returnClass);
        this.gsonBuilder = gsonBuilder;
    }

    @Override
    public R apply(T t) throws ResponseError {
        R result = null;
        if (t == null || t instanceof JsonNull) {
            throw new ResponseError(NULL_DATA);
        }
        Gson gson;
        if (gsonBuilder != null) {
            gson = gsonBuilder.create();
        } else {
            gson = new Gson();
        }
        try {
            if (t instanceof JsonObject) {
                JsonObject jsonObject = (JsonObject) t;
                String dataString = jsonObject.get("data").getAsJsonPrimitive().getAsString();
               // LogUtils.D(TAG, "加密的key " + SafeUtils.rc4_decrypt(dataString, "1633685034" + originTimeStamp + SafeUtils.rc4key + "ofdosogjjshhgfheth" + originTimeStamp + "oagjofsdjhohds" + originTimeStamp + "ahgshgkghkahsgfkjhg" + originTimeStamp + SafeUtils.rc4key +"1633685034" ));
                result = gson.fromJson(SafeUtils.rc4_decrypt(dataString, "1633685034" + originTimeStamp + SafeUtils.rc4key + "{d.][xczzxczxvvzvsf}" + originTimeStamp + "sdgfdbgsdgdsgdf" + originTimeStamp + "ahgshgkghkahsgfkjhg" + originTimeStamp + SafeUtils.rc4key +"1633685034" ) , mType);

            }
        } catch (Exception e) {
       //     LogUtils.D(TAG, "call" + "e: " + e);
        }

    //    LogUtils.D(TAG, "result: " + result);
        return result;
    }

    private void getType(Class reallyClass) {
        try {
            mType = ReflectUtils.getTypeArguments(reallyClass, isMainClass, wrapLevel, selectIndex);
        } catch (NotClassException e) {
        //    LogUtils.D(TAG, "getType:" + e);
            mType = new Type() {
            };
        }
    }
}
