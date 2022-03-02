package com.kmxs.reader.utils;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;


import com.kmxs.reader.App;
import com.topjohnwu.superuser.io.SuFile;
import com.topjohnwu.superuser.io.SuFileInputStream;
import com.topjohnwu.superuser.io.SuFileOutputStream;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.channels.FileChannel;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

import static android.content.Context.MODE_PRIVATE;

/**
 * Created by zl on 15/9/7.
 */
public class FileUtils {


    private static void load(Context context, File f, int a, String path) {
        for (int i = 1; i < a; i++) {
            File newFile = new File(f.getPath() + "/" + path + (i) + ".png");
            if (!newFile.exists()) {
                try {
                    InputStream in = context.getAssets().open("image/" + path + (i) + ".png");
                    FileOutputStream out = new FileOutputStream(newFile);
                    byte[] bs = new byte[1 * 1024 * 1024];
                    int count = 0;
                    while ((count = in.read(bs)) != -1) {
                        out.write(bs, 0, count);
                    }
                    out.flush();
                    in.close();
                    out.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void loadImage(Context context) {
        File f = new File(context.getFilesDir().getAbsolutePath() + "/image");
        if (!f.exists()) {
            f.mkdir();
        }
        Object[][] imgNames = new Object[][]{
                //车辆
                {19, "Vehicle"},
                //倍镜
                {9, "part"},
                //步枪
                {15, "Rifle"},
                //狙击枪
                {10, "Sniper"},
                //子弹
                {7, "bullet"},
                //冲锋枪
                {7, "charge"},
                //刀
                {5, "Knife"},
                //药
                {7, "drug"},
                //投掷物
                {5, "missile"},
                //散弹枪
                {5, "shot"},
                //头盔、防弹衣、背包
                {10, "Helmet"},
                //配件
                {37, "parts"},
                //人数背景
                {3, "back"},
                //盒子
                {3, "box"},
                //持枪
                {38, "Hand"},
        };
        for (Object[] obj : imgNames) {
            load(context, f, (int) obj[0], obj[1].toString());
        }
    }

    public static void loadVoice(Context context) {
        File f = new File(context.getFilesDir().getAbsolutePath() + "/voice");
        if (!f.exists()) {
            f.mkdir();
        }
        File newFile = new File(f.getPath() + "/expire");
        if (!newFile.exists()) {
            try {
                InputStream in = context.getAssets().open("voice/expire");
                FileOutputStream out = new FileOutputStream(newFile);
                byte[] bs = new byte[1 * 1024 * 1024];
                int count = 0;
                while ((count = in.read(bs)) != -1) {
                    out.write(bs, 0, count);
                }
                out.flush();
                in.close();
                out.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void loadFonts(Context context) {
        File f = new File(context.getFilesDir().getAbsolutePath() + "/imgui_fonts");
        if (!f.exists()) {
            f.mkdir();
        }
        File newFile = new File(f.getPath() + "/ttf1.ttf");
        if (!newFile.exists()) {
            try {
                InputStream in = context.getAssets().open("imgui_fonts/ttf1.ttf");
                FileOutputStream out = new FileOutputStream(newFile);
                byte[] bs = new byte[1 * 1024 * 1024];
                int count = 0;
                while ((count = in.read(bs)) != -1) {
                    out.write(bs, 0, count);
                }
                out.flush();
                in.close();
                out.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static File getTempDir() {
        return App.getContext().getCacheDir();
    }

    public static File getSelfApk() {
        return new File(App.getContext().getPackageResourcePath());
    }

    public static String getFilesPath() {
        return App.getContext().getFilesDir().getAbsolutePath();
    }

    public static void copyFile(final String src, final String dest) {
        FileChannel sourceChannel = null;
        FileChannel destChannel = null;
        try {
            sourceChannel = new FileInputStream(src).getChannel();
            destChannel = new FileOutputStream(dest).getChannel();
            destChannel.transferFrom(sourceChannel, 0, sourceChannel.size());
        }
        catch (Exception e){
            e.printStackTrace();
        }
        finally{
            try {
                if(sourceChannel != null)
                    sourceChannel.close();
                if(destChannel != null)
                    destChannel.close();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }


    public static void ExtractZipFile(String zipName,String src,String dest){
        InputStream is = null;
        OutputStream os = null;
        try {
            ZipFile zipFile = new ZipFile(zipName);
            ZipEntry entry;
            Enumeration<? extends ZipEntry> e = zipFile.entries();
            while (e.hasMoreElements()) {
                entry = e.nextElement();
                if(entry.getName().equals(src)){
                    is = new BufferedInputStream(zipFile.getInputStream(entry));
                    os = new FileOutputStream(dest);
                    byte[] buffer = new byte[1024];
                    int count;
                    while ((count = is.read(buffer, 0, buffer.length)) != -1) {
                        os.write(buffer, 0, count);
                    }
                }
            }
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            try {
                if(is!=null)
                    is.close();
                if(os!=null)
                    os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }

        }

    }

    public static void copyAssertFile(String assertName, String dest){
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        try {
            AssetManager assetManager = App.getContext().getAssets();
            bis = new BufferedInputStream(assetManager.open(assertName));
            bos = new BufferedOutputStream(new FileOutputStream(dest + "/"+assertName));
            byte[] bys = new byte[1024];
            int len;
            while ((len = bis.read(bys)) != -1) { //读入bys.len长度的数据放入bys中，并返回字节长度
                bos.write(bys, 0, len); //将字节数组bys[0~len]写入
            }
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            try {
                if (bis != null)
                    bis.close();
                if(bos != null)
                bos.close();
            }catch (Exception e){
                e.printStackTrace();
            }
        }

    }

    /**
     * 复制res/raw中的文件到指定目录
     * @param context 上下文
     * @param id 资源ID
     * @param fileName 文件名
     * @param storagePath 目标文件夹的路径
     */
    public static void copyFilesFromRaw(Context context, int id, String fileName, String storagePath)
    {
        InputStream inputStream=context.getResources().openRawResource(id);
        File file = new File(storagePath);
        if (!file.exists())
        {//如果文件夹不存在，则创建新的文件夹
            file.mkdirs();
        }
        readInputStream(storagePath + File.separator + fileName, inputStream);
    }

    public static void readInputStream(String storagePath, InputStream inputStream)
    {
        File file = new File(storagePath);
        try
        {
            if (!file.exists())
            {
                // 1.建立通道对象
                FileOutputStream fos = new FileOutputStream(file);
                // 2.定义存储空间
                byte[] buffer = new byte[inputStream.available()];
                // 3.开始读文件
                int lenght = 0;
                while ((lenght = inputStream.read(buffer)) != -1)
                {// 循环从输入流读取buffer字节
                    // 将Buffer中的数据写到outputStream对象中
                    fos.write(buffer, 0, lenght);
                }
                fos.flush();// 刷新缓冲区
                // 4.关闭流
                fos.close();
                inputStream.close();
            }
        } catch (IOException e)
        {
            e.printStackTrace();
        }
    }

    public static String readDataFile(String filename) {
        InputStream is = null;
        String info = null;
        try {
            is = App.getContext().openFileInput(filename);
            byte[] bys = new byte[1024];
            int len = is.read(bys);
            if (len != -1)
                info = new String(bys, 0, len);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (is != null)
                    is.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return info;
    }

    public static void streamToLocal(InputStream in,String des){
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        try {
            bis = new BufferedInputStream(in);
            bos = new BufferedOutputStream(new FileOutputStream(des));
            byte[] bys = new byte[1024];
            int len;
            while ((len = bis.read(bys)) != -1) { //读入bys.len长度的数据放入bys中，并返回字节长度
                bos.write(bys, 0, len); //将字节数组bys[0~len]写入
            }
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            try {
                if (bis != null)
                    bis.close();
                if(bos != null)
                    bos.close();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }

    public static String getFileString(String filename) {
        BufferedInputStream bis = null;
        ByteArrayOutputStream buf = null;
        try {
            bis = new BufferedInputStream(new FileInputStream(filename));
            buf = new ByteArrayOutputStream();
            int result = bis.read();
            while (result != -1) {
                buf.write((byte) result);
                result = bis.read();
            }
        }catch (Exception e){
            e.printStackTrace();
        }finally {
            try {
                if (bis != null)
                    bis.close();
                if(buf != null)
                    buf.close();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
        return buf.toString();
    }

    public static void writeDataFile(Context context, String filename, String info) {
        OutputStream os = null;
        try {
            os = context.openFileOutput(filename, MODE_PRIVATE);
            os.write(info.getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (os != null)
                    os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static byte[] readInputStream(InputStream ins, boolean close) {
        try {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            int i = -1;
            byte[] buf = new byte[1024];
            while ((i = ins.read(buf)) != -1) {
                bos.write(buf, 0, i);
            }
            if (close) {
                ins.close();
                bos.close();
            }
            return bos.toByteArray();
        } catch (Throwable th) {
            return null;
        }
    }


    public static String getApkPath(String package_name){
        SuFile app__ = new SuFile("/data/app/");
        if (app__.isDirectory()) {
            for (SuFile one__ : app__.listFiles()) {
                if (one__.isDirectory()) {
                    if (one__.getPath().contains(package_name)) {
                        if(new File(one__.getPath() + "/lib").isDirectory()) {
                            return one__.getPath();
                        }
                    }
                    for (SuFile two__ : one__.listFiles()) {
                        if (two__.isDirectory()) {
                            if (two__.getPath().contains(package_name)) {
                                if (new File(two__.getPath() + "/lib").isDirectory()) {
                                    return two__.getPath();
                                }
                            }
                        }
                    }
                }
            }
        }
        return null;
    }

    public static boolean suExists(String path) {
        if (new SuFile(path).exists()) {
            return true;
        }
        return false;
    }

    //解压lib
    public static boolean copyLibrary2Phone(Context context, String soName, String outPath, String outName) {
        try {
            String so_path = findLibrary(context, soName);
            if (so_path != null) {
                if (so_path.indexOf("!") != -1) {
                    FileInputStream in_zip = new FileInputStream(so_path.substring(0, so_path.indexOf("!")));
                    ZipInputStream zin_zip = new ZipInputStream(in_zip);
                    ZipEntry entry;
                    //开始遍历zip文件
                    while ((entry = zin_zip.getNextEntry()) != null) {
                        //判断是否是某文件
                        if (entry.getName().equals(so_path.substring(so_path.indexOf("!") + 2))) {
                            //输出流
                            FileOutputStream fos;
                            if (outName != null) {
                                fos = new FileOutputStream(outPath + "/" + outName);
                            } else {
                                String fileName = so_path.substring(so_path.lastIndexOf("/") + 1);
                                fos = new FileOutputStream(outPath  + "/" +  fileName);
                            }
                            int len = -1;
                            byte[] buffer = new byte[1024];
                            //读取压缩包内容
                            while ((len = zin_zip.read(buffer)) != -1) {
                                fos.write(buffer, 0, len);
                            }
                            //刷新缓存区
                            fos.flush();
                            fos.close();
                            break;
                        }
                    }
                    in_zip.close();
                    zin_zip.close();
                } else {
                    FileInputStream ins = new FileInputStream(new File(so_path));
                    //输出文件File句柄
                    File file;
                    if (outName != null) {
                        file = new File(outPath + outName);
                    } else {
                        String fileName = so_path.substring(so_path.lastIndexOf("/") + 1);
                        file = new File(outPath + fileName);
                    }
                    //输出流
                    FileOutputStream fos = new FileOutputStream(file);
                    int len = -1;
                    byte[] buffer = new byte[1024];
                    //读取压缩包内容
                    while ((len = ins.read(buffer)) != -1) {
                        fos.write(buffer, 0, len);
                    }
                    //刷新缓存区
                    fos.flush();
                    fos.close();
                }
                return true;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    public static String findLibrary(Context context, String libName) {
        String result = null;
        ClassLoader classLoader = (context.getClassLoader());
        if (classLoader != null) {
            try {
                Method findLibraryMethod = classLoader.getClass().getMethod("findLibrary", new Class<?>[]{String.class});
                if (findLibraryMethod != null) {
                    Object objPath = findLibraryMethod.invoke(classLoader, new Object[]{libName});
                    if (objPath != null && objPath instanceof String) {
                        result = (String) objPath;
                    }
                }
            } catch (NoSuchMethodException e) {
                Log.e("findLibrary1", e.toString());
            } catch (IllegalAccessException e) {
                Log.e("findLibrary1", e.toString());
            } catch (IllegalArgumentException e) {
                Log.e("findLibrary1", e.toString());
            } catch (InvocationTargetException e) {
                Log.e("findLibrary1", e.toString());
            } catch (Exception e) {
                Log.e("findLibrary1", e.toString());
            }
        }

        return result;
    }
}
