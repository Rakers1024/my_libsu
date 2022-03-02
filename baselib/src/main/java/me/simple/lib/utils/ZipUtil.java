package me.simple.lib.utils;

import android.os.Build;

import com.hzy.libp7zip.P7ZipApi;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipOutputStream;

/**
 * Created by wzj on 2017/11/27.
 */
public class ZipUtil {

    public static void UnZipFolder(String FILE_PATH, String outPath) throws Exception{
        File zipFile = new File(FILE_PATH);//FILE_PATH为压缩文件的路径
        ZipFile zf = new ZipFile(zipFile);
        for(Enumeration<?> entries = zf.entries(); entries.hasMoreElements();){//遍历压缩文件中的所有归档过的文件

            ZipEntry entry = ((ZipEntry)entries.nextElement());//默认为-1，next->下标0的位置
            InputStream is = zf.getInputStream(entry);
            String str = outPath + File.separator + entry.getName();
            str = new String(str.getBytes("8859_1"),"GBK");//压缩文件的编码是8859_1,此处可将其转换成指定的编码
            File desFile = new File(str);//其中单执行new file并不会在存储中创建文件或文件夹
            if(!desFile.exists()){
                File fileParentDir = desFile.getParentFile();
                if(!fileParentDir.exists()){
                    fileParentDir.mkdirs();//多级目录同时创建  怕文件归档在很多级目录下,或者文件需要在很多级未创建目录下，mkdir不会创建父目录，mkdirs会
                }
                desFile.createNewFile();//如果没有父目录将创建失败
            }
            FileOutputStream out = new FileOutputStream(desFile);
            byte buffer[] = new byte[1024*1024];
            int readLength;
            while((readLength = is.read(buffer)) > 0){
                out.write(buffer, 0, readLength);
            }
            is.close();
            out.close();
        }
    }


    public static void zip(String src,String dest) throws IOException {
        //定义压缩输出流
        ZipOutputStream out = null;
        try {
            //传入源文件
            File fileOrDirectory= new File(src);
            File outFile= new File(dest);
            //传入压缩输出流
            //创建文件前几级目录
            if (!outFile.exists()){
                File parentfile=outFile.getParentFile();
                if (!parentfile.exists()){
                    parentfile.mkdirs();
                }
            }
            //可以通过createNewFile()函数这样创建一个空的文件，也可以通过文件流的使用创建
            out = new ZipOutputStream(new FileOutputStream(outFile));
            //判断是否是一个文件或目录
            //如果是文件则压缩
            if (fileOrDirectory.isFile()){
                zipFileOrDirectory(out,fileOrDirectory, "");
            } else {
                //否则列出目录中的所有文件递归进行压缩

                File[]entries = fileOrDirectory.listFiles();
                for (int i= 0; i < entries.length;i++) {
                    zipFileOrDirectory(out,entries[i],fileOrDirectory.getName()+"/");//传入最外层目录名

                }
            }
        }catch(IOException ex) {
            ex.printStackTrace();
        }finally{
            if (out!= null){
                try {
                    out.close();
                }catch(IOException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }

    private static void zipFileOrDirectory(ZipOutputStream out, File fileOrDirectory, String curPath)throws IOException {
        FileInputStream in = null;
        try {
            //判断是否为目录
            if (!fileOrDirectory.isDirectory()){
                byte[] buffer= new byte[4096];
                int bytes_read;
                in= new FileInputStream(fileOrDirectory);//读目录中的子项
                //归档压缩目录
                ZipEntry entry = new ZipEntry(curPath + fileOrDirectory.getName());//压缩到压缩目录中的文件名字
                //getName() 方法返回的路径名的名称序列的最后一个名字，这意味着表示此抽象路径名的文件或目录的名称被返回。
                //将压缩目录写到输出流中
                out.putNextEntry(entry);//out是带有最初传进的文件信息，一直添加子项归档目录信息
                while ((bytes_read= in.read(buffer))!= -1) {
                    out.write(buffer,0, bytes_read);
                }
                out.closeEntry();
            } else {
                //列出目录中的所有文件
                File[]entries = fileOrDirectory.listFiles();
                for (int i= 0; i < entries.length;i++) {
                    //递归压缩
                    zipFileOrDirectory(out,entries[i],curPath + fileOrDirectory.getName()+ "/");//第一次传入的curPath是空字符串
                }//目录没有后缀所以直接可以加"/"
            }
        }catch(IOException ex) {
            ex.printStackTrace();
        }finally{
            if (in!= null){
                try {
                    in.close();
                }catch(IOException ex) {
                    ex.printStackTrace();
                }
            }
        }
    }



    /**
     * 解压文件
     *
     * @param zipPath 要解压的目标文件
     * @param descDir 指定解压目录
     * @return 解压结果：成功，失败
     */
    @SuppressWarnings("rawtypes")
    public static boolean decompressZip(String zipPath, String descDir) {
        return P7ZipApi.executeCommand(getExtractCmd(zipPath, descDir)) > 0;
    }

    /**
     * 解压文件
     *
     * @param zipfile 要解压的目标文件
     * @param descDir 指定解压目录
     * @param passwd  解压密码
     * @return 解压结果：成功，失败
     */
    public static boolean decompressZip(String zipfile, String descDir, String passwd) {
        return P7ZipApi.executeCommand(getExtractWithPasswordCmd(zipfile, descDir, passwd)) > 0;
    }

    public static String getExtractCmd(String archivePath, String outPath) {
        return String.format("7z x '%s' '-o%s' -aoa -mmt%s", archivePath, outPath, getNumberOfCPUCores());
    }

    public static String getExtractWithPasswordCmd(String archivePath, String outPath, String passwd) {
        return String.format("7z x '%s' -p%s '-o%s' -aoa -mmt%s", archivePath, passwd, outPath, getNumberOfCPUCores());
    }

    private static final int DEVICEINFO_UNKNOWN = 2;

    public static int getNumberOfCPUCores() {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.GINGERBREAD_MR1) {
            return 1;  //上面的意思就是2.3以前不支持多核,有些特殊的设备有双核...不考虑,就当单核!!
        }
        int cores;
        try {
            cores = new File("/sys/devices/system/cpu/").listFiles(CPU_FILTER).length;
        } catch (SecurityException e) {
            cores = DEVICEINFO_UNKNOWN;   //这个常量得自己约定
        } catch (NullPointerException e) {
            cores = DEVICEINFO_UNKNOWN;
        }
        return cores;
    }

    private static final FileFilter CPU_FILTER = new FileFilter() {
        @Override
        public boolean accept(File pathname) {
            String path = pathname.getName();
            //regex is slow, so checking char by char.
            if (path.startsWith("cpu")) {
                for (int i = 3; i < path.length(); i++) {
                    if (path.charAt(i) < '0' || path.charAt(i) > '9') {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
    };

}

