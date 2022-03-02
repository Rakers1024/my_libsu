//
// Created by admin on 2021/8/11.
//

#include <jni.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <cstring>
#include <string>
#include <string.h>

#include <android/log.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <set>
#include <string_view>
#include <dlfcn.h>
#include <pthread.h>
#include <imgui_draw_main.h>
//#include <bytehook.h>
#include "include/log.h"


extern "C"
{


jobject getContext(JNIEnv *env) {
    jclass activityThreadClz = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThreadMtd = env->GetStaticMethodID(activityThreadClz,
                                                                "currentActivityThread",
                                                                "()Landroid/app/ActivityThread;");
    jobject activityThread = env->CallStaticObjectMethod(activityThreadClz,
                                                         currentActivityThreadMtd);
    jmethodID getApplicationMtd = env->GetMethodID(activityThreadClz, "getApplication",
                                                   "()Landroid/app/Application;");
    return env->CallObjectMethod(activityThread, getApplicationMtd);
}

void showToast(JNIEnv *env, char *str) {
    jclass toastClz = env->FindClass("android/widget/Toast");
    jmethodID makeTextMtd = env->GetStaticMethodID(toastClz, "makeText",
                                                   "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jobject toast = env->CallStaticObjectMethod(toastClz, makeTextMtd, getContext(env),
                                                env->NewStringUTF(str), (jint) -1);
    jmethodID showMtd = env->GetMethodID(toastClz, "show", "()V");
    env->CallVoidMethod(toast, showMtd);
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
#ifndef NDEBUG
    showToast(env, "debug:数据注入成功");
#endif
    pthread_t t;
    pthread_create(&t, 0, main_thread, 0);
    LOGD("so被加载");

    return JNI_VERSION_1_6;
}



JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *unused) {

}

JNIEXPORT jint pandora_tcupload_callfun(int result, int a2) {
    return result;
}

JNIEXPORT jint
Java_com_tencent_pandora_libtcloudupload_TCloudUpload_nativePostUploadEvent(int a1, int a2, int a3,
                                                                            int a4, int a5,
                                                                            int a6) {
    int v6 = a1;
    int v7 = a4;
    int v8 = a3;
    int v9 = (*(int (**)(void)) (*(int *) a1 + 676))();

    return (*(int (__fastcall **)(int, int, int)) (*(int *) v6 + 680))(v6, a6, v9);
}
}
