//
// Created by WhiteSheng on 2022/2/11.
//
namespace PubgMhd {
    //获取剪贴板
    void getClipboardStr(char *str) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jstring clipboardStr = env->NewStringUTF(string("clipboard").c_str());
        jobject clipboard = env->CallObjectMethod(globalContext, getSystemService, clipboardStr);
        env->DeleteLocalRef(clipboardStr);

        jclass clipboardClass = env->GetObjectClass(clipboard);
        jmethodID getPrimaryClip = env->GetMethodID(clipboardClass, "getPrimaryClip", "()Landroid/content/ClipData;");
        jobject clip = env->CallObjectMethod(clipboard, getPrimaryClip);

        jclass clipClass = env->GetObjectClass(clip);
        jmethodID getItemAt = env->GetMethodID(clipClass, "getItemAt", "(I)Landroid/content/ClipData$Item;");
        jobject item = env->CallObjectMethod(clip, getItemAt, 0);

        jclass itemClass = env->GetObjectClass(item);
        jmethodID getText = env->GetMethodID(itemClass, "getText", "()Ljava/lang/CharSequence;");
        jobject text = env->CallObjectMethod(item, getText);

        jclass textClass = env->GetObjectClass(text);
        jmethodID toString = env->GetMethodID(textClass, "toString", "()Ljava/lang/String;");
        jstring jstr = (jstring) env->CallObjectMethod(text, toString);

        jstring2CStr(env, "UTF-8", jstr, str);

        env->DeleteLocalRef(jstr);
        env->DeleteLocalRef(clipboard);
        env->DeleteLocalRef(item);
        env->DeleteLocalRef(text);
    }

    //写入剪贴板
    void putClipboardStr(char *str) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jstring clipboardStr = env->NewStringUTF(string("clipboard").c_str());
        jobject clipboard = env->CallObjectMethod(globalContext, getSystemService, clipboardStr);
        env->DeleteLocalRef(clipboardStr);

        jclass clipClass = env->FindClass("android/content/ClipData");
        jmethodID newPlainText = env->GetStaticMethodID(clipClass, "newPlainText", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;");


        jstring lable = env->NewStringUTF(string("KeyCopy").c_str());

        jstring jstr = env->NewStringUTF(str);

        jobject clip = env->CallStaticObjectMethod(clipClass, newPlainText, lable, jstr);

        jclass clipboardClass = env->GetObjectClass(clipboard);
        jmethodID setPrimaryClip = env->GetMethodID(clipboardClass, "setPrimaryClip", "(Landroid/content/ClipData;)V");
        env->CallVoidMethod(clipboard, setPrimaryClip, clip);

        env->DeleteLocalRef(clipboard);
        env->DeleteLocalRef(clip);
    }

    void putIntValue(const char *key, int value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, env->NewStringUTF("tsui"), 0);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID edit = env->GetMethodID(spClass, "edit", "()Landroid/content/SharedPreferences$Editor;");
        jobject editor = env->CallObjectMethod(sp, edit);

        jclass editorClass = env->GetObjectClass(editor);

        jmethodID putInt = env->GetMethodID(editorClass, "putInt", "(Ljava/lang/String;I)Landroid/content/SharedPreferences$Editor;");
        jstring jkey = env->NewStringUTF(key);
        env->CallObjectMethod(editor, putInt, jkey, (jint)value);
        //释放内存
        env->DeleteLocalRef(jkey);

        jmethodID apply = env->GetMethodID(editorClass, "apply", "()V");
        env->CallVoidMethod(editor, apply);
        env->DeleteLocalRef(sp);
        env->DeleteLocalRef(editor);
    }

    void putFloatValue(const char *key, float value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, env->NewStringUTF("tsui"), 0);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID edit = env->GetMethodID(spClass, "edit", "()Landroid/content/SharedPreferences$Editor;");
        jobject editor = env->CallObjectMethod(sp, edit);

        jclass editorClass = env->GetObjectClass(editor);

        jmethodID putFloat = env->GetMethodID(editorClass, "putFloat", "(Ljava/lang/String;F)Landroid/content/SharedPreferences$Editor;");
        jstring jkey = env->NewStringUTF(key);
        env->CallObjectMethod(editor, putFloat, jkey, (jfloat)value);
        //释放内存
        env->DeleteLocalRef(jkey);

        jmethodID apply = env->GetMethodID(editorClass, "apply", "()V");
        env->CallVoidMethod(editor, apply);
        env->DeleteLocalRef(sp);
        env->DeleteLocalRef(editor);
    }

    void putBooleanValue(const char *key, bool value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, env->NewStringUTF("tsui"), 0);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID edit = env->GetMethodID(spClass, "edit", "()Landroid/content/SharedPreferences$Editor;");
        jobject editor = env->CallObjectMethod(sp, edit);

        jclass editorClass = env->GetObjectClass(editor);

        jmethodID putBoolean = env->GetMethodID(editorClass, "putBoolean", "(Ljava/lang/String;Z)Landroid/content/SharedPreferences$Editor;");
        jstring jkey = env->NewStringUTF(key);
        env->CallObjectMethod(editor, putBoolean, jkey, (jboolean)value);
        //释放内存
        env->DeleteLocalRef(jkey);

        jmethodID apply = env->GetMethodID(editorClass, "apply", "()V");
        env->CallVoidMethod(editor, apply);
        env->DeleteLocalRef(sp);
        env->DeleteLocalRef(editor);
    }

    void putStringValue(const char *key, const char *value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, env->NewStringUTF("tsui"), 0);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID edit = env->GetMethodID(spClass, "edit", "()Landroid/content/SharedPreferences$Editor;");
        jobject editor = env->CallObjectMethod(sp, edit);

        jclass editorClass = env->GetObjectClass(editor);

        jmethodID putString = env->GetMethodID(editorClass, "putString", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
        jstring jkey = env->NewStringUTF(key);
        jstring jvalue = env->NewStringUTF(value);
        env->CallObjectMethod(editor, putString, jkey, jvalue);
        //释放内存
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(jvalue);

        jmethodID apply = env->GetMethodID(editorClass, "apply", "()V");
        env->CallVoidMethod(editor, apply);

        env->DeleteLocalRef(sp);
        env->DeleteLocalRef(editor);
    }

    int getIntValue(const char *key, int value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jstring spName = env->NewStringUTF("tsui");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, spName, 0);
        env->DeleteLocalRef(spName);
        jclass spClass = env->GetObjectClass(sp);
        jmethodID getInt = env->GetMethodID(spClass, "getInt", "(Ljava/lang/String;I)I");

        jstring jkey = env->NewStringUTF(key);
        jint result = env->CallIntMethod(sp, getInt, jkey, (jint)value);
        //释放内存
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(sp);
        return result;
    }

    float getFloatValue(const char *key, float value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jstring spName = env->NewStringUTF("tsui");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, spName, 0);
        env->DeleteLocalRef(spName);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID getFloat = env->GetMethodID(spClass, "getFloat", "(Ljava/lang/String;F)F");

        jstring jkey = env->NewStringUTF(key);
        jfloat result = env->CallFloatMethod(sp, getFloat, jkey, (jfloat)value);
        //释放内存
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(sp);
        return result;
    }

    bool getBooleanValue(const char *key, bool value) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jstring spName = env->NewStringUTF("tsui");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, spName, 0);
        env->DeleteLocalRef(spName);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID getBoolean = env->GetMethodID(spClass, "getBoolean", "(Ljava/lang/String;Z)Z");

        jstring jkey = env->NewStringUTF(key);
        jboolean result = env->CallBooleanMethod(sp, getBoolean, jkey, (jboolean) value);
        //释放内存
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(sp);
        return result;
    }

    void getStringValue(const char *key, const char *value, char *buf) {
        jclass contextClass = env->GetObjectClass(globalContext);
        jmethodID getSharedPreferences = env->GetMethodID(contextClass, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jstring spName = env->NewStringUTF("tsui");
        jobject sp = env->CallObjectMethod(globalContext, getSharedPreferences, spName, 0);
        env->DeleteLocalRef(spName);

        jclass spClass = env->GetObjectClass(sp);
        jmethodID getString = env->GetMethodID(spClass, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        jstring jkey = env->NewStringUTF(key);
        jstring jvalue = env->NewStringUTF(value);
        jstring result = (jstring) env->CallObjectMethod(sp, getString, jkey, jvalue);
        //释放内存
        env->DeleteLocalRef(jkey);
        env->DeleteLocalRef(jvalue);

        jstring2CStr(env, "UTF-8", result, buf);
        env->DeleteLocalRef(result);

        env->DeleteLocalRef(sp);
    }
}