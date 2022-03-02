package com.kmxs.reader.greendao.db;

import android.annotation.SuppressLint;
import android.content.Context;

import com.kmxs.reader.greendao.DaoMaster;
import com.kmxs.reader.greendao.DaoSession;

import org.greenrobot.greendao.database.Database;

public class DbManager {

    // 是否加密
    public static final boolean ENCRYPTED = true;


    @SuppressLint("StaticFieldLeak")
    private static volatile DbManager mDbManager;
    @SuppressLint("StaticFieldLeak")
    private static DaoMaster.DevOpenHelper mDevOpenHelper;
    private static DaoMaster mDaoMaster;
    private static DaoSession mDaoSession;
    private final String a = "";

    private DbManager(Context context, String dbName, String passwprd) {
        // 初始化数据库信息
        mDevOpenHelper = new DaoMaster.DevOpenHelper(context, dbName);
        getDaoMaster(context, dbName, passwprd);
        getDaoSession(context, dbName, passwprd);
    }

    public static DbManager getInstance(Context context, String dbName, String passwprd) {
        if (null == mDbManager) {
            synchronized (DbManager.class) {
                if (null == mDbManager) {
                    mDbManager = new DbManager(context, dbName, passwprd);
                }
            }
        }
        return mDbManager;
    }


    public static Database getReadableDatabase(Context context, String dbName, String passwprd) {
        if (null == mDevOpenHelper) {
            getInstance(context, dbName, passwprd);
        }
        if (ENCRYPTED) {//加密
            return mDevOpenHelper.getEncryptedReadableDb(passwprd);
        } else {
            return mDevOpenHelper.getReadableDb();
        }
    }


    public static Database getWritableDatabase(Context context, String dbName, String passwprd) {
        if (null == mDevOpenHelper) {
            getInstance(context, dbName, passwprd);
        }
        if (ENCRYPTED) {
            return mDevOpenHelper.getEncryptedWritableDb(passwprd);
        } else {
            return mDevOpenHelper.getWritableDb();
        }
    }

    public static DaoMaster getDaoMaster(Context context, String dbName, String passwprd) {
        if (null == mDaoMaster) {
            synchronized (DbManager.class) {
                if (null == mDaoMaster) {
                    mDaoMaster = new DaoMaster(getWritableDatabase(context, dbName, passwprd));
                }
            }
        }
        return mDaoMaster;
    }


    public static DaoSession getDaoSession(Context context, String dbName, String passwprd) {
        if (null == mDaoSession) {
            synchronized (DbManager.class) {
                mDaoSession = getDaoMaster(context,dbName,passwprd).newSession();
                //mDaoSession = getDaoMaster(context, dbName, passwprd).newDevSession(context, dbName);
            }
        }

        return mDaoSession;
    }
}