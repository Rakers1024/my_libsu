package me.simple.lib.utils;
import me.simple.lib.BuildConfig;

public class SafeUtils {

    public static String appid = null;
    public static String rc4key = null;
    public static boolean islogin = false;
    public static boolean isCharge = true;

    public static String rc4_decrypt(String data, String key) {
        if (data == null || key == null) {
            return "";
        }
        try {
            return new String(RC4Base(HexString2Bytes(data), key), "GBK");
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    public static String RC4(String keys, String encrypt) {
        char[] keyBytes = new char[256];
        char[] cypherBytes = new char[256];

        for (int i = 0; i < 256; ++i) {
            keyBytes[i] = keys.charAt(i % keys.length());
            cypherBytes[i] = (char) i;
        }

        int jump = 0;
        for (int i = 0; i < 256; ++i) {
            jump = (jump + cypherBytes[i] + keyBytes[i]) & 0xFF;
            char tmp = cypherBytes[i]; // Swap:
            cypherBytes[i] = cypherBytes[jump];
            cypherBytes[jump] = tmp;
        }

        int i = 0;
        jump = 0;
        String Result = "";
        for (int x = 0; x < encrypt.length(); ++x) {
            i = (i + 1) & 0xFF;
            char tmp = cypherBytes[i];
            jump = (jump + tmp) & 0xFF;
            char t = (char) ((tmp + cypherBytes[jump]) & 0xFF);
            cypherBytes[i] = cypherBytes[jump]; // Swap:
            cypherBytes[jump] = tmp;

            try {
                Result += new String(new char[] { (char) (encrypt.charAt(x) ^ cypherBytes[t]) });
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return Result;
    }

    public static byte[] RC4Base(byte[] input, String mKkey) {
        int x = 0;
        int y = 0;
        byte[] key = initKey(mKkey);
        byte[] result = new byte[input.length];
        for (int i = 0; i < input.length; i++) {
            x = (x + 1) & 0xff;
            y = ((key[x] & 0xff) + y) & 0xff;
            byte tmp = key[x];
            key[x] = key[y];
            key[y] = tmp;
            result[i] = (byte) (input[i] ^ key[((key[x] & 0xff) + (key[y] & 0xff)) & 0xff]);
        }
        return result;
    }

    /**
     * RC4加密,解密
     * @author YOLANDA
     * @param aInput 要加密或者解密的文字
     * @param aKey 加密或者解密的key
     * @return
     */
    public static String parseOrCreateRC4(String aInput, String aKey) {
        int[] iS = new int[256];
        byte[] iK = new byte[256];

        for (int i = 0; i < 256; i++)
            iS[i] = i;

        int j = 1;

        for (short i = 0; i < 256; i++) {
            iK[i] = (byte) aKey.charAt((i % aKey.length()));
        }

        j = 0;

        for (int i = 0; i < 255; i++) {
            j = (j + iS[i] + iK[i]) % 256;
            int temp = iS[i];
            iS[i] = iS[j];
            iS[j] = temp;
        }

        int i = 0;
        j = 0;
        char[] iInputChar = aInput.toCharArray();
        char[] iOutputChar = new char[iInputChar.length];
        for (short x = 0; x < iInputChar.length; x++) {
            i = (i + 1) % 256;
            j = (j + iS[i]) % 256;
            int temp = iS[i];
            iS[i] = iS[j];
            iS[j] = temp;
            int t = (iS[i] + (iS[j] % 256)) % 256;
            int iY = iS[t];
            char iCY = (char) iY;
            iOutputChar[x] = (char) (iInputChar[x] ^ iCY);
        }
        return new String(iOutputChar);
    }

    private static byte[] initKey(String aKey) {
        try {
            byte[] b_key = aKey.getBytes("GBK");
            byte[] state = new byte[256];
            for (int i = 0; i < 256; i++) {
                state[i] = (byte) i;
            }
            int index1 = 0;
            int index2 = 0;
            if (b_key == null || b_key.length == 0) {
                return null;
            }
            for (int i2 = 0; i2 < 256; i2++) {
                index2 = ((b_key[index1] & 0xff) + (state[i2] & 0xff) + index2) & 0xff;
                byte tmp = state[i2];
                state[i2] = state[index2];
                state[index2] = tmp;
                index1 = (index1 + 1) % b_key.length;
            }
            return state;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }


    private static byte[] HexString2Bytes(String src) {
        try {
            int size = src.length();
            byte[] ret = new byte[(size / 2)];
            byte[] tmp = src.getBytes("GBK");
            for (int i = 0; i < size / 2; i++) {
                ret[i] = uniteBytes(tmp[i * 2], tmp[(i * 2) + 1]);
            }
            return ret;
        } catch (Exception e) {
            e.printStackTrace();
            return new byte[0];
        }
    }

    public static String getTimeStamp(int type) {
        if (type == 2) {
            return ShellUtils.execCmd("date +%s", false, true).successMsg;
        }
        return ShellUtils.execCmd("date +%N", false, true).successMsg;
    }
    private static byte uniteBytes(byte src0, byte src1) {
        return (byte) (((char) (((char) Byte.decode("0x" + new String(new byte[]{src0})).byteValue()) << 4)) ^ ((char) Byte.decode("0x" + new String(new byte[]{src1})).byteValue()));
    }


}
