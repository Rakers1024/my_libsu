package com.kmxs.reader.utils;

import android.app.Activity;

import android.util.Log;


import com.kmxs.reader.App;
import com.wind.meditor.core.FileProcesser;
import com.wind.meditor.property.AttributeItem;
import com.wind.meditor.property.ModificationProperty;
import com.wind.meditor.utils.NodeValue;

import net.fornwall.apksigner.KeyStoreFileManager;
import net.fornwall.apksigner.ZipSigner;

import com.kmxs.reader.R;

import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.X509Certificate;

import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ShellUtils;
import me.simple.lib.utils.ZipUtil;

/*
 **注意事项
 * 1、Exception from call site #0 bootstrap method
 * 配置java1.8
 *
 * */
public class ApkHelper {

    private final String packageName;
    private final String appName;
    private final String workDir;
    private final String oldApk;
    private final String newApk;
    private final String signedApk;
    private final String oldXml;
    private final String newXml;


    public ApkHelper() {
        packageName = RandomUtils.getRandomPkgName();
        appName = RandomUtils.getRandomAppName();
        workDir = FileUtils.getTempDir().getAbsolutePath();
        oldApk = workDir + "/old.apk";
        newApk = workDir + "/new.apk";
        signedApk = workDir + "/signed.apk";
        oldXml = workDir + "/old.xml";
        newXml = workDir + "/new.xml";
    }
  //  authorities
    public void installSignedApk(Activity context) {
        //准备xml、apk、keystore
        FileUtils.copyFile(FileUtils.getSelfApk().getAbsolutePath(), oldApk);
//        FileUtils.copyAssertFile("keystore",workDir);
        FileUtils.copyFilesFromRaw(App.getContext(), R.raw.keystore, "keystore", workDir);
       // FileUtils.ExtractZipFile(oldApk, "AndroidManifest.xml", oldXml);
        ZipUtil.decompressZip(oldApk, "AndroidManifest.xml", oldXml);
        //配置xml属性
        ModificationProperty property = new ModificationProperty();
        property
                .addApplicationAttribute(new AttributeItem(NodeValue.LABEL, appName))
                .addManifestAttribute(new AttributeItem(NodeValue.Manifest.PACKAGE, packageName)

                        .setNamespace(null));
        // 处理manifest文件方法
        FileProcesser.processManifestFile(oldXml, newXml, property);

        // 处理得到的未签名的apk
        FileProcesser.processApkFile(oldApk, newApk, property);
        //签名apk
        signApk(newApk, signedApk);
        //安装apk
        installApk(context, signedApk);
    }

    public void installApk(Activity context, String apkPath) {
        ShellUtils.CommandResult result = ShellUtils.execCmd("pm install " + apkPath, true);

    }

    public void signApk(String src, String dest) {
        String keystorePath = workDir + "/keystore";
        LogUtils.D("ApkHelper","path:"+keystorePath);
        String keyPassword = "ORNGBF";
        try {
            KeyStore keyStore = KeyStoreFileManager.loadKeyStore(keystorePath, keyPassword.toCharArray());
            String alias = keyStore.aliases().nextElement();
            X509Certificate publicKey = (X509Certificate) keyStore.getCertificate(alias);
            PrivateKey privateKey = (PrivateKey) keyStore.getKey(alias, keyPassword.toCharArray());
            ZipSigner.signZip(publicKey, privateKey, "SHA1withRSA", src, dest);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
