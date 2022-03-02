package com.kmxs.reader.greendao.db;

import android.content.Context;

import com.kmxs.reader.bean.Config;
import com.kmxs.reader.greendao.ConfigDao;

import org.greenrobot.greendao.query.QueryBuilder;

import java.util.List;

public class ConfigDaoOpe {
    private static final String DB_NAME = "config.db";
    public static final String PASSWPRD = "bf6666";


    public static void insertData(Context context, Config config) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().insertOrReplace(config);
    }


    public static void insertData(Context context, List<Config> list) {
        if (null == list || list.size() <= 0) {
            return;
        }
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().insertOrReplaceInTx(list);
    }

    public static void saveData(Context context, Config config) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().save(config);
    }


    public static void deleteData(Context context, Config config) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().delete(config);
    }


    public static void deleteByKeyData(Context context, long id) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().deleteByKey(id);
    }


    public static void deleteAllData(Context context) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().deleteAll();
    }


    public static void updateData(Context context, Config config) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().update(config);
    }

    public static List<Config> queryAll(Context context) {
        QueryBuilder<Config> builder = DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().queryBuilder();
        return builder.build().list();
    }

    public static void hasKey(Context context, Config config) {
        DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().hasKey(config);
    }


    public static List<Config> queryForId(Context context, long id) {
        QueryBuilder<Config> builder = DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().queryBuilder();
        return builder.where(ConfigDao.Properties.Id.eq(id)).list();
    }

    public static List<Config> queryForName(Context context, String name) {
        QueryBuilder<Config> builder = DbManager.getDaoSession(context, DB_NAME, PASSWPRD).getConfigDao().queryBuilder();
        return builder.where(ConfigDao.Properties.Name.eq(name)).list();
    }

}
