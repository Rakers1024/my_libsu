package me.simple.lib.utils;


public class EncrypUtils {

    public static String getTimeStamp(int type) {
        if (type == 2) {
            return ShellUtils.execCmd("date +%s", false, true).successMsg;
        }
        return ShellUtils.execCmd("date +%N", false, true).successMsg;
    }

    public static String oOOOooooOOoOOoo(String data, String key) {
        if (data == null || key == null) {
            return "";
        }
        try {
            StringBuffer buffer = new StringBuffer();
            for (byte byte0 : RC4Base(data.getBytes("GBK"), key)) {
                String temp = Integer.toHexString(byte0 & 255);
                if (temp.length() == 1) {
                    buffer.append("0");
                }
                buffer.append(temp);
            }
            return new String(buffer).toUpperCase();
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
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

    private static byte[] RC4Base(byte[] input, String mKkey) {
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

    private static byte uniteBytes(byte src0, byte src1) {
        return (byte) (((char) (((char) Byte.decode("0x" + new String(new byte[]{src0})).byteValue()) << 4)) ^ ((char) Byte.decode("0x" + new String(new byte[]{src1})).byteValue()));
    }

}
