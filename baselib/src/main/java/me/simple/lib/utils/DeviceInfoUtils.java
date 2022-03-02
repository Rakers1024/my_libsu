package me.simple.lib.utils;

import android.content.Context;
import android.text.TextUtils;

import com.topjohnwu.superuser.io.SuFileInputStream;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.simpleframework.xml.Root;
import org.simpleframework.xml.Serializer;
import org.simpleframework.xml.core.Persister;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Random;

import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.ShellUtils;

public class DeviceInfoUtils {

    private static final String header = "<?xml version='\\''1.0'\\'' encoding='\\''UTF-8'\\'' standalone='\\''yes'\\'' ?>\n";
    private static final String TAG = "DeviceInfoUtils";

    public class SettingsItem {

        @Attribute
        private String id;
        @Attribute
        private String name;
        @Attribute
        private String value;
        @Attribute(name = "package")
        private String packageName;
        @Attribute
        private String defaultValue;
        @Attribute
        private boolean defaultSysSet;
        @Attribute
        private String tag;
        private transient String appName;

        public String getAppName() {
            return appName;
        }

        public void setAppName(String appName) {
            this.appName = appName;
        }

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getValue() {
            return value;
        }

        public void setValue(String value) {
            this.value = value.toLowerCase();
        }

        public String getPackageName() {
            return packageName;
        }

        public void setPackageName(String packageName) {
            this.packageName = packageName;
        }

        public String getDefaultValue() {
            return defaultValue;
        }

        public void setDefaultValue(String defaultValue) {
            this.defaultValue = defaultValue.toLowerCase();
        }

        public boolean isDefaultSysSet() {
            return defaultSysSet;
        }

        public void setDefaultSysSet(boolean defaultSysSet) {
            this.defaultSysSet = defaultSysSet;
        }

        public String getTag() {
            return tag;
        }

        public void setTag(String tag) {
            this.tag = tag;
        }

        public boolean canChange() {
//    return !"android".equals(packageName);
            return true;
        }

        public String toXmlString() {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append("  <setting id=\"");
            stringBuilder.append(id);
            stringBuilder.append("\" name=\"");
            stringBuilder.append(name);
            stringBuilder.append("\" value=\"");
            stringBuilder.append(value);
            stringBuilder.append("\" package=\"");
            stringBuilder.append(packageName);
            stringBuilder.append("\" defaultValue=\"");
            stringBuilder.append(defaultValue);
            stringBuilder.append("\" defaultSysSet=\"");
            stringBuilder.append(defaultSysSet);
            stringBuilder.append("\" tag=\"");
            stringBuilder.append(tag);
            stringBuilder.append("\" />\n");
            return stringBuilder.toString();
        }
    }

    @Root(name = "settings")
    public class SettingsFile {

        @Attribute
        private String version;
        @ElementList(entry = "setting", inline = true)
        private List<SettingsItem> items;

        public String getVersion() {
            return version;
        }

        public void setVersion(String version) {
            this.version = version;
        }

        public List<SettingsItem> getItems() {
            return items;
        }

        public void setItems(List<SettingsItem> items) {
            this.items = items;
        }
    }


    /* 获取主板ID */
    public static String getDeviceId() {
        String getDeviceId = "service call iphonesubinfo 1 | grep -o '[0-9a-f]\\{8\\} ' | tail -n+3 | while read a; do echo -n \\\\u${a:4:4}\\\\u${a:0:4}; done";
        getDeviceId = "cat /sys/devices/soc0/serial_number";
        String imei = ShellUtils.execCmd(getDeviceId, true, true).successMsg;
        String ANDROID_ID = ShellUtils.execCmd("settings get secure android_id", true, true).successMsg;
        return TextUtils.isEmpty(imei) ? ANDROID_ID : imei;
    }


    public static String getDeviceId(Context context) {
        String DeviceId = "";
        String cmd = "service call iphonesubinfo 3 i32 1 | grep -o '[0-9a-f]\\{8\\} ' | tail -n+3 | while read a; do echo -n \\\\u${a:4:4}\\\\u${a:0:4}; done";
        String imei1 = ShellUtils.execadb(new String[]{cmd}, true).successMsg;

        String cmd2 = "service call iphonesubinfo 3 i32 2 | grep -o '[0-9a-f]\\{8\\} ' | tail -n+3 | while read a; do echo -n \\\\u${a:4:4}\\\\u${a:0:4}; done";
        String imei2 = ShellUtils.execadb(new String[]{cmd2}, true).successMsg;

        if (TextUtils.isEmpty(imei1) && TextUtils.isEmpty(imei2)) {
            DeviceId = getDeviceId();
            return DeviceId;
        }

        if (TextUtils.isEmpty(imei1)) {
            return imei2;
        }

        if (TextUtils.isEmpty(imei2)) {
            return imei1;
        }
        return imei1;
    }

    public static String GenerateRandomAndroidId() {
        StringBuilder randomString = new StringBuilder();
        String st = "12AB345CD67890EF";
        Random rn = new Random();
        for (int i = 0; i < 16; i++) {
            randomString.append(st.charAt(rn.nextInt(st.length())));
        }
        return randomString.toString();
    }

    private static void execute(String value) {
        final String command = "echo '" + value + "' > /data/system/users/0/settings_ssaid.xml";
        ShellUtils.execCmd(command,true,true);
    }

    public static void ChangeAndroidId(String PackageName, String AndroidId) {
        InputStream inputStream = null;
        Serializer serializer = new Persister();
        try {
            inputStream = SuFileInputStream.open("/data/system/users/0/settings_ssaid.xml");
            SettingsFile readFile = serializer.read(SettingsFile.class, inputStream);
            String lowerCase = AndroidId.toLowerCase();
            LogUtils.D(TAG, "修改设备号:" + lowerCase);
            if (readFile != null) {
                for (SettingsItem settingsItem : readFile.getItems()) {
                    if (settingsItem.getPackageName().equals(PackageName)) {
                        settingsItem.setValue(lowerCase);
                        settingsItem.setDefaultValue(lowerCase);
                        break;
                    }
                }
                StringBuilder builder = new StringBuilder();
                builder.append(header);
                builder.append("<settings version=\"-1\">\n");
                for (SettingsItem item : readFile.getItems()) {
                    builder.append(item.toXmlString());
                }
                builder.append("</settings>");
                execute(builder.toString());
                LogUtils.D(TAG, "修改完成");
            }
        } catch (Throwable e) {
            LogUtils.E(TAG, e.toString());
        } finally {
            try {
                if (inputStream != null) {
                    inputStream.close();
                }
            } catch (IOException e) {
                LogUtils.E(TAG, e.toString());
            }
        }
    }
}
