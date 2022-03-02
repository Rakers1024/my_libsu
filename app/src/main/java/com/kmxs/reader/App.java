package com.kmxs.reader;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.ContextThemeWrapper;

import com.kmxs.reader.R;

import com.kmxs.reader.activity.LogActivity;
import com.kmxs.reader.bean.Config;
import com.kmxs.reader.greendao.db.ConfigDaoOpe;
import com.kmxs.reader.utils.AppConfig;
import com.kmxs.reader.utils.AppConfigManger;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import me.simple.lib.BaseContentApp;
import me.simple.lib.net.RetrofitClient;
import me.simple.lib.utils.CrashHandlerUtil;
import me.simple.lib.utils.ReflectionApiCheck;

public class App extends BaseContentApp {

    @SuppressLint("StaticFieldLeak")
    private static Context mContextThemeWrapper = null;
    public static boolean isUpdata = true;
    //public static boolean isUpdata = true;
    @Override
    public void onCreate() {
        super.onCreate();
        CrashHandlerUtil.getInstance().init(mContext, LogActivity.class);
        AppConfig mAppConfig = AppConfigManger.getInstance().readAppConfig();
        RetrofitClient.getInstance().OOoooOooOOooOO(mAppConfig.HOST, "z2mmpHs8Xxw6X7iWMGK4KZCH6TJA4FZS", mAppConfig.appid,mAppConfig.rc4key,"jdgaljgladjglasjgoaig","agjugearfksdhgieravflda","sdjfjlksdglkjdlsfjgljdsfj");
        //过魅族权限;
        ReflectionApiCheck.exemptAll();
    }


    public synchronized static Context getContext() {
        if (mContextThemeWrapper == null) {
            mContextThemeWrapper = new ContextThemeWrapper(mContext, R.style.AppTheme2);
        }
        return mContext;
    }

    public static String ReadConfig(String key, String def) {
        List<Config> configs = ConfigDaoOpe.queryAll(mContext);
        for (int i = 0; i < configs.size(); i++) {
            String KEY = configs.get(i).getName();
            String VALUE = configs.get(i).getValue();
            if (key.equals(KEY)) {
                return VALUE;
            }
        }
        return def;
    }

    public static void SaveConfig(String key, Object value) {
        List<Config> configs = ConfigDaoOpe.queryAll(mContext);
        for (int i = 0; i < configs.size(); i++) {
            String KEY = configs.get(i).getName();
            if (key.equals(KEY)) {
                ConfigDaoOpe.deleteByKeyData(mContext, configs.get(i).getId());
                break;
            }
        }
        ConfigDaoOpe.saveData(mContext, new Config(key, value.toString()));
    }

    public static boolean HasConfig(String key) {
        List<Config> configs = ConfigDaoOpe.queryAll(mContext);
        for (int i = 0; i < configs.size(); i++) {
            String KEY = configs.get(i).getName();
            if (key.equals(KEY)) {
                return true;
            }
        }
        return false;
    }

    public static boolean isReadMsg(String id) {
        String str_id = ReadConfig("msg", "");
        String[] str_list = str_id.split("#");
        List<String> list = Arrays.asList(str_list);
        List<String> temp = new ArrayList<>(list);
        if (temp.contains(id)) {
            return true;
        }
        temp.add(id);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < temp.size(); i++) {
            if (i < temp.size() - 1) {
                sb.append(temp.get(i));
                sb.append("#");
            } else {
                sb.append(temp.get(i));
            }
        }
        SaveConfig("msg", sb.toString());
        return false;
    }
}
