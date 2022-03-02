//
// Created by admin on 2022/2/13.
//

#include <jni.h>
#include <zconf.h>
#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../include/log.h"
#include "../include/jni.h"
#include "../imgui/backends/imgui_impl_android.h"
#include "imgui_draw_main.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <android/native_window.h>
#include <linux/ptrace.h>
#include <android/native_window_jni.h>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>
#include <dlfcn.h>
#include <chrono>
#include <backends/imgui_impl_opengl3.h>
#include "dobby.h"
#include "GameUtils.h"
#include "https.h"
#include "RC4Utils.h"
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <fake_dlfcn.h>
#include <xhook.h>
#include <MemoryUtils.h>
#include <IORelocator.h>
#include <sys/system_properties.h>
#include <syscall.h>
#include <MapsRedirector.h>
#include <sys/prctl.h>
#include <whale.h>

//禁用弹窗
float height, width;
MyInputEvent inputEvent;
using namespace std;
uintptr_t my_libUE4 = 0;


/*************************** 绘制配置 ***********************************/
bool showHealth = true, showLine = true, showCrosshair = true, showDis = true, showInfo = true, showRadar = true;
bool showBone = true, showBox = true;
bool showItem = true, show556 = true, show762 = true, showRifle = true, showMirror = true, showExpansion = true, showDrug = true, showArmor = true, showSubmachine = true, showSniper = true, showOtherParts = true;
bool showVehicle = true, showGrenade = true, showWeapons = true, isVisibility = true;
bool showChest = true, showDrop = true, showVehicleInfo = true, isObjAimed = true;
bool showTouzhi = true, showSandan = true, isSelectedAim = true, showVehicleBox = true;
bool aimShortDis = false;
bool showRange = true;
bool showImg = true, showText = true;
bool showWarning = true, showTarget = true;
float radarLocation = 100.f;
int espWidth = 1, boneWidth = 3, boxWidth = 3, lineWidth = 3;
int boxType = 1;
bool aimFire = true, aimOpen = false;
int boxRange = 200;
int boxShowType = 1, dropShowType = 1;
bool showBoxRange = false, isRunningLoc = false;
int touchMode = 0;
ImVec4 selectedAim_color = Color.Yellow, bone_color = Color.Red, box_color = Color.Red, line_color = Color.Red, fallen_color = Color.Yellow, visibility_color = Color.White, ai_color = Color.Green;
ImVec4 health_color{65.f / 255.f, 186.f / 255.f, 217.f / 255.f, 150 / 255.f};
ImVec4 drop_color = ImVec4(255 / 255.f, 0 / 255.f, 0 / 255.f, 255.f / 255.f);
ImVec4 aim_color = ImVec4(255 / 255.f, 0 / 255.f, 0 / 255.f, 255 / 255.f);
//药品
ImVec4 drug_color = ImVec4(255 / 255.f, 105 / 255.f, 180 / 255.f, 255.f / 255.f);
//背敌预警颜色
ImVec4 warning_color = ImVec4(255 / 255.f, 255 / 255.f, 0 / 255.f, 200.f / 255.f);
//载具颜色
ImVec4 vehicle_color = ImVec4(255.f / 255.f, 255 / 255.f, 0 / 255.f, 255 / 255.f);
//装备颜色
ImVec4 armor_color = ImVec4(255 / 255.f, 215 / 255.f, 0 / 255.f, 255.f / 255.f);
//狙击枪颜色
ImVec4 snipe_color = ImVec4(144 / 255.f, 238 / 255.f, 0 / 255.f, 255.f / 255.f);
//子弹颜色
ImVec4 color_bullet = ImVec4(255 / 255.f, 255 / 255.f, 0 / 255.f, 255.f / 255.f);
//冲锋枪颜色
ImVec4 submachine_color = ImVec4(255 / 255.f, 110 / 255.f, 180 / 255.f, 255.f / 255.f);
//投掷物颜色
ImVec4 missile_color = ImVec4(255 / 255.f, 0 / 255.f, 0 / 255.f, 255.f / 255.f);
//步枪
ImVec4 rifle_color = ImVec4(255 / 255.f, 0 / 255.f, 0 / 255.f, 255.f / 255.f);
//散弹枪颜色
ImVec4 color_shot = ImVec4(255 / 255.f, 255 / 255.f, 0 / 255.f, 255.f / 255.f);
//倍镜颜色
ImVec4 mirror_color = ImVec4{255.f / 255.f, 144.f / 255.f, 0.f / 255.f, 255.f / 255.f};
//配件颜色
ImVec4 expansion_color = ImVec4{208.f / 255.f, 128.f / 255.f, 114.f / 255.f, 255.f / 255.f};
//其他配件颜色
ImVec4 otherparts_color = ImVec4{245.f / 255.f, 245.f / 255.f, 245.f / 255.f, 255.f / 255.f};
//雷达
bool showRadarDis = true;
bool showRadarBox = true;
int radarType = 2;
bool visibilityAim = false, dieNoAim = false, lineAim = true, aiAim = true, isDlip = false, isShowLoc = false, aimbot = true, my_aimbot = true;
int aimLocation = 3, aimType = 2, aimedMode = 3, aimChoose = 1;
float aimRange = 150.f, touchJitter = 6.f, touchSpeed = 50.f, touchPress = -10, touchLocX = 50, touchLocY = 0, aimPress = 45.0f, aimSpeed = 10.0f, aimAnticipation = 10.0f;
float max_shake = 0;
float SlideRanges = 200;
static char g_LogTag[] = "GameInject";
bool isShowMenu = false;
float draw_scale = 1.5f;
//烟雾不瞄
float projSomoke = 200;
bool isProjSomoke = true, isProjSomokeRange = true;
Vector2A radarOffset = {0, 0};
int radarOffsetX = 400;
int radarOffsetY = 300;
float radarSize = 100;
/*************************** 绘制配置 ***********************************/
//悬浮图标
TextureInfo imageButton;
TextureInfo imageAimButtonOpen;
TextureInfo imageAimButtonOff;
static bool my_window_open = false;
static bool isImageDown = false;
static bool isAimImageDown = false;
static bool my_aim_window_open = false;
ImVec2 suspensionAimPos;
ImVec2 suspensionPos;
android_app *g_App = 0;
//游戏分辨率
int glWidth = 0, glHeight = 0;
//屏幕分辨率
int screenWidth = -1, screenHeight = -1;
float density = -1;
//登录的一些配置
static bool initImGui = false;
static bool initLogin = false;
static bool initLogin2 = false;
static bool initUpdateList = false;
static int loginCode = 1000;
static string GNativeAndroidApp;
static string my_token;
static string initState;
static string my_user;
//程序到期时间
static int app_effective = 1647273600;
//配置目录
string initPath = "/storage/emulated/0/Download/IMGUI/";

//声明读取图片函数
TextureInfo createTexture(const string &ImagePath);

//声明初始化加载图片函数
void InitTexture();
/*
 static void InitMenu(bool *open, JNIEnv env);


int handleInputEvent(MyInputEvent inputEvent) {
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_imguiInit(JNIEnv *env, jobject thiz) {
    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGui_ImplOpenGL3_Init("#version 100");
    ImGui_ImplOpenGL3_Init("#version 300 es");

    ImGuiIO &io = ImGui::GetIO();

    // char path[256];
    // sprintf(path, "/storage/emulated/0/imgui_init/apple_font.ttf");
    // io.Fonts->AddFontFromFileTTF(path, 33.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_navateInputEvent(JNIEnv *env, jobject thiz, jobject event) {
    jclass eventClss = env->GetObjectClass(event);
    jmethodID methodId = env->GetMethodID(eventClss, "getAction", "()I");
    inputEvent.Action = env->CallIntMethod(event, methodId);

    methodId = env->GetMethodID(eventClss, "getRawX", "()F");
    inputEvent.x = env->CallFloatMethod(event, methodId);

    methodId = env->GetMethodID(eventClss, "getRawY", "()F");
    inputEvent.y = env->CallFloatMethod(event, methodId);

    int event_pointer_index = (inputEvent.Action & 65280) >> 8;

    methodId = env->GetMethodID(eventClss, "getToolType", "(I)I");
    inputEvent.ToolType = env->CallIntMethod(event, methodId, event_pointer_index);

    methodId = env->GetMethodID(eventClss, "getButtonState", "()I");
    inputEvent.ButtonState = env->CallIntMethod(event, methodId);

    methodId = env->GetMethodID(eventClss, "getAxisValue", "(II)F");
    inputEvent.AXIS_VSCROLL = env->CallFloatMethod(event, methodId, 9, event_pointer_index);
    inputEvent.AXIS_HSCROLL = env->CallFloatMethod(event, methodId, 10, event_pointer_index);

    handleInputEvent(inputEvent);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_imguiResize(JNIEnv *env, jobject thiz, jint x,
                                                jint y) {

    width = x, height = y;
    //纹理
    //ImGui_ImplAndroidGLES2_CreateDeviceObjects();
    LOGD("width:%f height:%f", width, height);
    //初始化颜色
    ImGui::StyleColorsLight();

    glViewport(0, 0, width, height);
    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2(width, height);

    LOGD("加载完成..imguiResize");
}


uint64_t upTime;
static double g_Time = 0.0;
//g_Time = 0.0;

void msleep(double m) {
    usleep(m * 1000.0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_imguiRenderer(JNIEnv *env, jobject thiz) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    uint64_t currentTime = tp.count();
    ImGui::GetIO().DeltaTime =
            upTime == 0 ? 1.0f / 60.f : (float) ((currentTime - upTime)) / 1000000;
    upTime = currentTime;

    // InitMenu();
    //  ImGui::ShowDemoWindow();
    ImGui::Render();
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //ImGui_ImplAndroidGLES2_RenderDrawLists(ImGui::GetDrawData());
}

ImVec2 screenSize;

ImVec2 getScreenSize(JNIEnv *env, jclass windowManagerClass, jobject windowManager) {
    jclass pointClass = env->FindClass("android/graphics/Point");
    jmethodID pointInit = env->GetMethodID(pointClass, "<init>", "()V");
    jobject point = env->NewObject(pointClass, pointInit);

    jmethodID getDefaultDisplay = env->GetMethodID(windowManagerClass, "getDefaultDisplay",
                                                   "()Landroid/view/Display;");
    jobject display = env->CallObjectMethod(windowManager, getDefaultDisplay);

    jclass displayClass = env->GetObjectClass(display);
    jmethodID getRealSize = env->GetMethodID(displayClass, "getRealSize",
                                             "(Landroid/graphics/Point;)V");
    env->CallVoidMethod(display, getRealSize, point);

    jfieldID xField = env->GetFieldID(pointClass, "x", "I");
    jint x = env->GetIntField(point, xField);

    jfieldID yField = env->GetFieldID(pointClass, "y", "I");
    jint y = env->GetIntField(point, yField);

    ImVec2 result;
    if (x > y) {
        result.x = x;
        result.y = y;
    } else {
        result.x = y;
        result.y = x;
    }
    return result;
}

jobject createLayoutParams(JNIEnv *env, int width, int height) {
    jclass lpClass = env->FindClass("android/view/WindowManager$LayoutParams");
    jmethodID lpInit = env->GetMethodID(lpClass, "<init>", "()V");
    jobject lp = env->NewObject(lpClass, lpInit);



    jfieldID flagsField = env->GetFieldID(lpClass, "flags", "I");
    env->SetIntField(lp, flagsField, 512 | 8 | 16 | 32 | 256);

    jfieldID gravityField = env->GetFieldID(lpClass, "gravity", "I");
    env->SetIntField(lp, gravityField, 8388611);

    jfieldID formatField = env->GetFieldID(lpClass, "format", "I");
    env->SetIntField(lp, formatField, 1);

    jfieldID widthField = env->GetFieldID(lpClass, "width", "I");
    env->SetIntField(lp, widthField, width);

    jfieldID heightField = env->GetFieldID(lpClass, "height", "I");
    env->SetIntField(lp, heightField, height);

    jfieldID xField = env->GetFieldID(lpClass, "x", "I");
    env->SetIntField(lp, xField, 0);

    jfieldID yField = env->GetFieldID(lpClass, "y", "I");
    env->SetIntField(lp, yField, 0);

    return lp;
}


extern "C" JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_createDrawWindow(JNIEnv *env, jobject, jobject context,
                                                     jobject glView) {
    //获取window对象

    jclass contextClass = env->GetObjectClass(context);
    jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService",
                                                  "(Ljava/lang/String;)Ljava/lang/Object;");

    jstring windowStr = env->NewStringUTF("window");
    jobject windowManager = env->CallObjectMethod(context, getSystemService, windowStr);
    env->DeleteLocalRef(windowStr);

    jclass windowManagerClass = env->GetObjectClass(windowManager);
    screenSize = getScreenSize(env, windowManagerClass, windowManager);
    //创建布局参数
    jobject lp = createLayoutParams(env, screenSize.x, screenSize.y);

    jmethodID addView = env->GetMethodID(windowManagerClass, "addView",
                                         "(Landroid/view/View;Landroid/view/ViewGroup$LayoutParams;)V");
    env->CallVoidMethod(windowManager, addView, glView, lp);
    LOGD("创建成功");
}




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

 extern "C"
JNIEXPORT void JNICALL
Java_com_test_xposedhook_XposedHook_readDataInit(JNIEnv *env, jobject thiz) {
    // TODO: implement readDataInit()
   // showToast(env, "注入游戏");
    pthread_t t;
    pthread_create(&t, 0, main_thread, 0);
}

*/

/**
 * 保存配置
 */
void SaveIni() {
    FILE *fp;
    char path[256];
    sprintf(path, "%simgui_config",initPath.c_str());

    if ((fp = fopen(path, "wt+")) != nullptr) {
        char temp[4096];
        sprintf(temp,
                "显示方框=%d\n方框类型=%d\n显示骨骼=%d\n预判方框=%d\n显示血量=%d\n显示射线=%d\n显示距离=%d\n显示准心=%d\n显示信息=%d\n显示雷达=%d\n手持武器=%d\n预警提示=%d\n掩体判断=%d\n被瞄预警=%d\n选中变色=%d\n显示载具=%d\n载具信息=%d",
                showBox, boxType, showBone, isRunningLoc, showHealth, showLine, showDis,
                showCrosshair, showInfo, showRadar, showWeapons,
                showGrenade, isVisibility, isObjAimed, isSelectedAim, showVehicle, showVehicleInfo);
        fputs(temp, fp);


        sprintf(temp,
                "\n显示物品=%d\n显示盒子=%d\n显示空投=%d\n显示步枪=%d\n显示冲锋枪=%d\n显示狙击枪=%d\n显示子弹=%d\n显示倍镜=%d\n显示快扩=%d\n其他配件=%d\n显示药品=%d\n显示防具=%d\n显示投掷物=%d\n显示散弹枪=%d",
                showItem, showChest, showDrop, showRifle, showSubmachine, showSniper, show556,
                showMirror, showExpansion, showOtherParts, showDrug, showArmor, showTouzhi,
                showSandan);
        fputs(temp, fp);
        sprintf(temp,
                "\n开启自瞄=%d\n自瞄类型=%d\n掩体自瞄=%d\n自瞄线条=%d\n人机瞄准=%d\n屏幕翻转=%d\n触摸位置=%d\n倒地瞄准=%d\n自瞄抖动=%0.f\n预判速度=%0.f\n横轴位置=%0.f\n纵轴位置=%0.f\n压枪力度=%0.f\n自瞄压枪=%0.f\n自瞄速度=%0.f\n自瞄预判=%0.f\n自瞄模式=%d\n自瞄选中=%d\n自瞄位置=%d\n自瞄范围=%0.f\n显示自瞄范围=%d",
                aimbot, aimType, visibilityAim, lineAim, aiAim, isDlip, isShowLoc, dieNoAim,
                touchJitter, touchSpeed, touchLocX, touchLocY, touchPress, aimPress, aimSpeed,
                aimAnticipation, aimedMode, aimChoose, aimLocation, aimRange, showRange);
        fputs(temp, fp);

        sprintf(temp,
                "\n显示文字=%d\n显示图片=%d\n显示雷达距离=%d\n雷达类型=%d",
                showImg, showText, showRadarDis, radarType);
        fputs(temp, fp);

        //线条页
        sprintf(temp,
                "\n方框粗细=%d\n骨骼粗细=%d\n射线粗细=%d",
                boxWidth, boneWidth, lineWidth);
        fputs(temp, fp);

        //盒子设置
        sprintf(temp,
                "\n显示盒子范围=%d\n范围=%d\n盒子显示类型=%d\n空投显示类型=%d",
                showBoxRange, boxRange, boxShowType, dropShowType);
        fputs(temp, fp);
        //颜色配置
        sprintf(temp,
                "\n载具颜色=%0.f,%0.f.%0.f;%0.f\n背敌顔色=%0.f,%0.f.%0.f;%0.f\n步枪顔色=%0.f,%0.f.%0.f;%0.f\n冲锋枪顔色=%0.f,%0.f.%0.f;%0.f\n狙击枪顔色=%0.f,%0.f.%0.f;%0.f\n倍镜顔色=%0.f,%0.f.%0.f;%0.f\n配件顔色=%0.f,%0.f.%0.f;%0.f\n其他配件顔色=%0.f,%0.f.%0.f;%0.f\n药品顔色=%0.f,%0.f.%0.f;%0.f\n防具顔色=%0.f,%0.f.%0.f;%0.f\n投掷物顔色=%0.f,%0.f.%0.f;%0.f\n散弹枪顔色=%0.f,%0.f.%0.f;%0.f\n子弹顔色=%0.f,%0.f.%0.f;%0.f\n射线顔色=%0.f,%0.f.%0.f;%0.f\n方框顔色=%0.f,%0.f.%0.f;%0.f\n骨骼顔色=%0.f,%0.f.%0.f;%0.f\n倒地顔色=%0.f,%0.f.%0.f;%0.f\n人机顔色=%0.f,%0.f.%0.f;%0.f\n掩体顔色=%0.f,%0.f.%0.f;%0.f\n血条顔色=%0.f,%0.f.%0.f;%0.f\n准心顔色=%0.f,%0.f.%0.f;%0.f",
                vehicle_color.x * 255.f, vehicle_color.y * 255.f, vehicle_color.z * 255.f,
                vehicle_color.w * 255.f,
                warning_color.x * 255.f, warning_color.y * 255.f, warning_color.z * 255.f,
                warning_color.w * 255.f,
                rifle_color.x * 255.f, rifle_color.y * 255.f, rifle_color.z * 255.f,
                rifle_color.w * 255.f,
                submachine_color.x * 255.f, submachine_color.y * 255.f, submachine_color.z * 255.f,
                submachine_color.w * 255.f,
                snipe_color.x * 255.f, snipe_color.y * 255.f, snipe_color.z * 255.f,
                snipe_color.w * 255.f,
                mirror_color.x * 255.f, mirror_color.y * 255.f, mirror_color.z * 255.f,
                mirror_color.w * 255.f,
                expansion_color.x * 255.f, expansion_color.y * 255.f, expansion_color.z * 255.f,
                expansion_color.w * 255.f,
                otherparts_color.x * 255.f, otherparts_color.y * 255.f, otherparts_color.z * 255.f,
                otherparts_color.w * 255.f,
                drug_color.x * 255.f, drug_color.y * 255.f, drug_color.z * 255.f,
                drug_color.w * 255.f,
                armor_color.x * 255.f, armor_color.y * 255.f, armor_color.z * 255.f,
                armor_color.w * 255.f,
                missile_color.x * 255.f, missile_color.y * 255.f, missile_color.z * 255.f,
                missile_color.w * 255.f,
                color_shot.x * 255.f, color_shot.y * 255.f, color_shot.z * 255.f,
                color_shot.w * 255.f,
                color_bullet.x * 255.f, color_bullet.y * 255.f, color_bullet.z * 255.f,
                color_bullet.w * 255.f,
                line_color.x * 255.f, line_color.y * 255.f, line_color.z * 255.f,
                line_color.w * 255.f,
                box_color.x * 255.f, box_color.y * 255.f, box_color.z * 255.f, box_color.w * 255.f,
                bone_color.x * 255.f, bone_color.y * 255.f, bone_color.z * 255.f,
                bone_color.w * 255.f,
                fallen_color.x * 255.f, fallen_color.y * 255.f, fallen_color.z * 255.f,
                fallen_color.w * 255.f,
                ai_color.x * 255.f, ai_color.y * 255.f, ai_color.z * 255.f, ai_color.w * 255.f,
                visibility_color.x * 255.f, visibility_color.y * 255.f, visibility_color.z * 255.f,
                visibility_color.w * 255.f,
                health_color.x * 255.f, health_color.y * 255.f, health_color.z * 255.f,
                health_color.w * 255.f,
                selectedAim_color.x * 255.f, selectedAim_color.y * 255.f,
                selectedAim_color.z * 255.f, selectedAim_color.w * 255.f
        );

        fputs(temp, fp);

        sprintf(temp,
                "\n触摸模式=%d\n触摸范围=%0.f\n载具方框=%d\n横扫=%0.f\n烟雾大小=%0.f\n烟雾不瞄=%d\n烟雾范围显示=%d\n雷达X=%d\n雷达Y=%d\n雷达大小=%f\n显示外框=%d\n自瞄开关=%d",
                touchMode, SlideRanges, showVehicleBox, max_shake, projSomoke, isProjSomoke,
                isProjSomokeRange, radarOffsetX, radarOffsetY, radarSize, showRadarBox, my_aimbot);
        fputs(temp, fp);


        //玩家顔色=%0.f,%0.f.%0.f
    }
    fclose(fp);
}

/**
 * 读取配置
 */
void GetIni() {
    FILE *fp;
    char path[256];
    sprintf(path, "%simgui_config",initPath.c_str());
    if ((fp = fopen(path, "r")) != nullptr) {
        char temp[4096];
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showBox = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            boxType = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showBone = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isRunningLoc = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showHealth = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showLine = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showDis = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showCrosshair = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showInfo = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showRadar = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showWeapons = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showGrenade = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isVisibility = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isObjAimed = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isSelectedAim = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showVehicle = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showVehicleInfo = atoi(strstr(temp, "=") + 1);
        //物资
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showItem = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showChest = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showDrop = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showRifle = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showSubmachine = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showSniper = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            show556 = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showMirror = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showExpansion = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showOtherParts = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showDrug = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showArmor = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showTouzhi = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showSandan = atoi(strstr(temp, "=") + 1);

        //物资结束



        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimbot = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimType = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            visibilityAim = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            lineAim = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aiAim = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isDlip = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isShowLoc = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            dieNoAim = atoi(strstr(temp, "=") + 1);


        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchJitter = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchSpeed = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchLocX = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchLocY = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchPress = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimPress = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimSpeed = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimAnticipation = atof(strstr(temp, "=") + 1);

        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimedMode = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimChoose = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimLocation = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            aimRange = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showRange = atoi(strstr(temp, "=") + 1);


        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showImg = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showText = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showRadarDis = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            radarType = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            boxWidth = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            boneWidth = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            lineWidth = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showBoxRange = atof(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            boxRange = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            boxShowType = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            dropShowType = atoi(strstr(temp, "=") + 1);
        //设置




        if (fgets(temp, sizeof(temp), fp) != nullptr)
            vehicle_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                   atoi(strstr(temp, ",") + 1) / 255.f,
                                   atoi(strstr(temp, ".") + 1) / 255.f,
                                   atoi(strstr(temp, ";") + 1) / 255.f);

        if (fgets(temp, sizeof(temp), fp) != nullptr)
            warning_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                   atoi(strstr(temp, ",") + 1) / 255.f,
                                   atoi(strstr(temp, ".") + 1) / 255.f,
                                   atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            rifle_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                 atoi(strstr(temp, ",") + 1) / 255.f,
                                 atoi(strstr(temp, ".") + 1) / 255.f,
                                 atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            submachine_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                      atoi(strstr(temp, ",") + 1) / 255.f,
                                      atoi(strstr(temp, ".") + 1) / 255.f,
                                      atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            snipe_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                 atoi(strstr(temp, ",") + 1) / 255.f,
                                 atoi(strstr(temp, ".") + 1) / 255.f,
                                 atoi(strstr(temp, ";") + 1) / 255.f);

        if (fgets(temp, sizeof(temp), fp) != nullptr)
            mirror_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                  atoi(strstr(temp, ",") + 1) / 255.f,
                                  atoi(strstr(temp, ".") + 1) / 255.f,
                                  atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            expansion_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                     atoi(strstr(temp, ",") + 1) / 255.f,
                                     atoi(strstr(temp, ".") + 1) / 255.f,
                                     atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            otherparts_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                      atoi(strstr(temp, ",") + 1) / 255.f,
                                      atoi(strstr(temp, ".") + 1) / 255.f,
                                      atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            drug_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                atoi(strstr(temp, ",") + 1) / 255.f,
                                atoi(strstr(temp, ".") + 1) / 255.f,
                                atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            armor_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                 atoi(strstr(temp, ",") + 1) / 255.f,
                                 atoi(strstr(temp, ".") + 1) / 255.f,
                                 atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            missile_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                   atoi(strstr(temp, ",") + 1) / 255.f,
                                   atoi(strstr(temp, ".") + 1) / 255.f,
                                   atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            color_shot = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                atoi(strstr(temp, ",") + 1) / 255.f,
                                atoi(strstr(temp, ".") + 1) / 255.f,
                                atoi(strstr(temp, ";") + 1) / 255.f);

        if (fgets(temp, sizeof(temp), fp) != nullptr)
            color_bullet = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                  atoi(strstr(temp, ",") + 1) / 255.f,
                                  atoi(strstr(temp, ".") + 1) / 255.f,
                                  atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            line_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                atoi(strstr(temp, ",") + 1) / 255.f,
                                atoi(strstr(temp, ".") + 1) / 255.f,
                                atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            box_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                               atoi(strstr(temp, ",") + 1) / 255.f,
                               atoi(strstr(temp, ".") + 1) / 255.f,
                               atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            bone_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                atoi(strstr(temp, ",") + 1) / 255.f,
                                atoi(strstr(temp, ".") + 1) / 255.f,
                                atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            fallen_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                  atoi(strstr(temp, ",") + 1) / 255.f,
                                  atoi(strstr(temp, ".") + 1) / 255.f,
                                  atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            ai_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                              atoi(strstr(temp, ",") + 1) / 255.f,
                              atoi(strstr(temp, ".") + 1) / 255.f,
                              atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            visibility_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                      atoi(strstr(temp, ",") + 1) / 255.f,
                                      atoi(strstr(temp, ".") + 1) / 255.f,
                                      atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            health_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                  atoi(strstr(temp, ",") + 1) / 255.f,
                                  atoi(strstr(temp, ".") + 1) / 255.f,
                                  atoi(strstr(temp, ";") + 1) / 255.f);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            selectedAim_color = ImVec4(atoi(strstr(temp, "=") + 1) / 255.f,
                                       atoi(strstr(temp, ",") + 1) / 255.f,
                                       atoi(strstr(temp, ".") + 1) / 255.f,
                                       atoi(strstr(temp, ";") + 1) / 255.f);


        if (fgets(temp, sizeof(temp), fp) != nullptr)
            touchMode = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            SlideRanges = atoi(strstr(temp, "=") + 1);

        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showVehicleBox = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            max_shake = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            projSomoke = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isProjSomoke = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            isProjSomokeRange = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            radarOffsetX = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            radarOffsetY = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            radarSize = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            showRadarBox = atoi(strstr(temp, "=") + 1);
        if (fgets(temp, sizeof(temp), fp) != nullptr)
            my_aimbot = atoi(strstr(temp, "=") + 1);
        fclose(fp);
    }
}

/**
 * 弹窗
 * @param str
 * @param isExit
 */
void showAlert(const char *str, bool isExit) {
    ImGui::Begin("提示", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2{(float) glWidth / 2, 200});
    ImGui::SetWindowFontScale(2.0f);
    ImGui::SetWindowPos(
            ImVec2(glWidth / 2 - ImGui::GetWindowWidth() / 2,
                   glHeight / 2 - ImGui::GetWindowHeight() / 2));
    if (!isExit) {
        ImGui::Text("%s\n程序將在10秒后关闭", str);
    } else {
        ImGui::Text("%s", str);
    }
    ImGui::End();
}

/**
 * 验证码校验
 * @return
 */
bool showTipAlert() {
    switch (loginCode) {
        case 1000:
            showAlert("等待加载游戏数据", true);
            return false;
        case 1001:
            showAlert("登录失败,TOKEN不存在或已失效,请重新登录", true);
            return false;
        case 1002:
            showAlert("账号被禁用", true);
            return false;
        case 1003:
            showAlert("非法的服务器请求", true);
            return false;
        case 1004:
            showAlert("程序失效,请检测更新", true);
            return false;
        case 1006:
            showAlert("请求超时,检查你的网络", true);
            return false;
        case 1005:
            showAlert("错误的校验码,请重新登录", true);
            return false;
        case 1008:
            showAlert("错误的游戏数据,您是否尝试破解", true);
            return false;
        case 1009:
            showAlert("错误的游戏数据,请重新登录打开", true);
            return false;
        case 1007:
            return true;
        default:
            return false;
    }
}

/**
 * 心跳验证线程
 * @param heartbeat
 * @return
 */
int safeEffective(bool heartbeat) {
    /* 获取时间戳 */

    time_t t;
    time(&t);
    string url =
            "http://183.131.79.130:5455/?act=OoOoOoodfgsdnnkdffjsfsdf534ghgf5dsag12OooOoo0O0oOo0O0oO0o0O0o0o0O000&appid=10000&token=" +
            my_token + "&t=" + to_string(t) + "&type=9998" + "&usertype=" + my_user;

    string strResponse = http_get(url);
    string decryption3, decryption2;

    char *key = "{gjgsoagjsoajga==}";
    char *key2 = "{sdjfldsjglj==}";
    char *key3 = "{idjgldsjglsj==}";

    OoOoOooOoOOo(key3, decryption2, decryption2, key, key2, (char *) strResponse.c_str(), '{', '}',
                 'o', decryption3, decryption2, decryption2, decryption2);

    //登录失败
    if (strstr(decryption.c_str(), "123456789o") || strstr(decryption.c_str(), "4848484o")) {
        LOGD("登录失败");
        loginCode = 1001;
        return -1;
    }
    //账号被禁用
    if (strstr(decryption.c_str(), "46461919o")) {
        LOGD("账号被禁用");
        loginCode = 1002;
        return -1;
    }
    string OoOOoOoOiOOoOo = strstrstr(decryption.c_str(), "OoOOoOoOiOOoOo:", "\n", false);
    string OoOOoOoOiOOoo = "183.131.79.130";
    if (!strstr(OoOOoOoOiOOoOo.c_str(), OoOOoOoOiOOoo.c_str())) { return -1; }
    int va = atoi(strstrstr(decryption.c_str(), "Oo0o0O0o:", "\n", false));
    int timeTmp = atoi(strstrstr(decryption.c_str(), "time:", "\n", false));
    string tmp_homeid = strstrstr(decryption.c_str(), "home_id:", "\n", false);
    string tmp_home = strstrstr(decryption.c_str(), "important_data:", "\n", false);
    if (!heartbeat) {
        if (strlen((char *) tmp_homeid.c_str()) <= 0) {
            loginCode = 1008;
            return -1;
        }
        int pid = 0;
        pid = atoi(tmp_homeid.c_str());
        if (pid == 0) {
            loginCode = 1008;
            return -1;
        }
        int my_pid = getpid();
        LOGD("读取地址:%s pid:%d 本地地址:%lx my_pid:%d", tmp_home.c_str(), pid, my_libUE4, my_pid);
        if (my_pid != (pid - 164513248)) {
            LOGE("进程数据错误");
            loginCode = 1008;
            return -1;
        }
        char addr_tmp[32] = "";
        sprintf(addr_tmp, "%lx", my_libUE4 + 0x12589);
        if (!strstr(addr_tmp, tmp_home.c_str())) {
            loginCode = 1008;
            LOGE("模块数据错误");
            return -1;
        }
    }
    time_t t1;
    time(&t1);
    //软件有效期
    if ((int) t1 > app_effective || (int) t1 > timeTmp) {
        LOGD("程序过期");
        loginCode = 1004;
        return -1;
    }
    //软件校验
    if (va != 589478912) {
        LOGD("校验错误");
        loginCode = 1005;
        return -1;
    } else {
        loginCode = 1007;
    }
    int timeStamp = atoi(strstrstr(decryption.c_str(), "timeStamp:", "\n", false));
    //软件超时
    if (timeStamp - t1 > 100) {
        LOGD("请求超时：%s", strResponse.c_str());
        loginCode = 1006;
        return -1;
    }
    return 1;
}
[[noreturn]] void *heartbeatThread(void *) {
    prctl(PR_SET_NAME, "heartbeatThread");
    while (true) {
        sleep(600);
        safeEffective(true);
    }
}


/**
 * hook 游戏触摸
 * @param app
 * @param inputEvent
 * @return
 */
int32_t (*orig_onInputEvent)(struct android_app *app, AInputEvent *inputEvent);
int32_t onInputEvent(struct android_app *app, AInputEvent *inputEvent) {
    if (initImGui) {
        ImGui_ImplAndroid_HandleInputEvent(inputEvent, {(float) screenWidth / (float) glWidth,
                                                        (float) screenHeight / (float) glHeight});
    }
    return orig_onInputEvent(app, inputEvent);
}

/**
 * hook _eglSwapBuffers 进行绘制
 * @param dpy
 * @param surface
 * @return
 */
EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {

    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    //游戏分辨率
    if (glWidth <= 0 || glHeight <= 0)
        return orig_eglSwapBuffers(dpy, surface);

    if (!g_App)
        return orig_eglSwapBuffers(dpy, surface);

    if (g_App->onInputEvent != onInputEvent) {
        //hook触摸输入
        orig_onInputEvent = decltype(orig_onInputEvent)(g_App->onInputEvent);
        g_App->onInputEvent = onInputEvent;
    }

    if (!initImGui) {
        //屏幕分辨率
        screenWidth = ANativeWindow_getWidth(g_App->window);
        screenHeight = ANativeWindow_getHeight(g_App->window);
        density = AConfiguration_getDensity(g_App->config);
        LOGD("hook成功 x:%d y:%d density:%f", screenWidth, screenHeight, density);
        LOGD("hook成功 游戏x:%d 游戏y:%d ", glWidth, glHeight);

        ImGui::CreateContext();

        ImGuiStyle &style = ImGui::GetStyle();
        //窗口圆角
        style.WindowRounding = 6.3f;
        style.FrameRounding = 6.f;//2.3f;//框架圆角
        style.ScrollbarRounding = 0;
        style.ScrollbarSize /= 2;
        style.ScaleAllSizes(std::max(1.0f, density / 120.0f));

        // style.ScaleAllSizes(std::max(1.0f, density / 120.0f));
        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");
        ImGuiIO &io = ImGui::GetIO();
        //配置Windows只从标题栏移动
        // io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.IniFilename = NULL;
        //设置主题
        ImGui::StyleColorsLight();

        char path[256];
        sprintf(path, "%s/apple_font.ttf",initPath.c_str());
        io.Fonts->AddFontFromFileTTF(path, 22.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        //读取配置
        InitTexture();
        GetIni();
        imageButton = createTexture(initPath + "image/logo.png");
        imageAimButtonOpen = createTexture(initPath + "/image/aimopen.png");
        imageAimButtonOff = createTexture(initPath + "/image/aimoff.png");

        //IM_ASSERT(font != NULL);
        startIOHook();

        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);              // background color

        initImGui = true;
    }
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    aimType = 2;
    if (!my_aimbot) {
        my_aim_window_open = true;
        aimbot = false;
    }
    if (showTipAlert()) {
        //更新数据 心跳 线程
        if (!initUpdateList) {
            pthread_t update_list, heartbeat;
            pthread_create(&update_list, nullptr, updateDataList, nullptr);
            pthread_create(&heartbeat, nullptr, heartbeatThread, nullptr);
            initUpdateList = true;
        }
        //二次校验
        if (loginCode == 1007) {
            if (!initLogin2) {
                safeEffective(false);
                initLogin2 = true;
            }
            if (showTipAlert()) {
                //读游戏
                createDataList();
                //悬浮球
                if (ImGui::Begin("##悬浮球", nullptr,
                                 ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
                    //是否按住编辑等
                    if (ImGui::IsItemActive()) {
                        if (!isImageDown) {
                            //按下
                            isImageDown = true;
                            suspensionPos = ImGui::GetWindowPos();
                        }
                    } else if (isImageDown) {
                        //未按下
                        isImageDown = false;
                        if (suspensionPos.x == ImGui::GetWindowPos().x &&
                            ImGui::GetWindowPos().y == suspensionPos.y) {
                            //菜单切换
                            my_window_open = !my_window_open;
                        }
                    }
                    if (!my_window_open) {
                        ImGui::Image(imageButton.textureId, ImVec2{75, 75}, ImVec2{0, 0},
                                     ImVec2{1, 1});
                    }
                    if (ImGui::GetWindowPos().x<0 || ImGui::GetWindowPos().y<
                            0 || ImGui::GetWindowPos().x>(glWidth - ImGui::GetWindowWidth()) ||
                                                ImGui::GetWindowPos().y>(
                            glHeight - ImGui::GetWindowHeight())) {
                        ImGui::SetWindowPos(ImVec2{glWidth / 3.f, glHeight / 4.f});
                    }
                }
                ImGui::End();
                //自瞄
                if (my_aimbot) {
                    if (ImGui::Begin("##自瞄", nullptr,
                                     ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar |
                                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
                        //是否按住编辑等
                        if (ImGui::IsItemActive()) {
                            if (!isAimImageDown) {
                                //按下
                                isAimImageDown = true;
                                suspensionAimPos = ImGui::GetWindowPos();
                            }
                        } else if (isAimImageDown) {
                            //未按下
                            isAimImageDown = false;
                            if (suspensionAimPos.x == ImGui::GetWindowPos().x &&
                                ImGui::GetWindowPos().y == suspensionAimPos.y) {
                                //自瞄切换
                                my_aim_window_open = !my_aim_window_open;
                                aimbot = !aimbot;
                            }
                        }
                        if (!my_aim_window_open) {
                            ImGui::Image(imageAimButtonOpen.textureId, ImVec2{75, 75}, ImVec2{0, 0},
                                         ImVec2{1, 1});
                        } else {
                            ImGui::Image(imageAimButtonOff.textureId, ImVec2{75, 75}, ImVec2{0, 0},
                                         ImVec2{1, 1});
                        }
                        if (ImGui::GetWindowPos().x<0 || ImGui::GetWindowPos().y<
                                0 || ImGui::GetWindowPos().x>(glWidth - ImGui::GetWindowWidth()) ||
                                                    ImGui::GetWindowPos().y>(
                                glHeight - ImGui::GetWindowHeight())) {
                            ImGui::SetWindowPos(ImVec2{glWidth / 3.f, glHeight / 4.f});
                        }
                    }
                    ImGui::End();
                }
                //菜单
                ImGui::SetNextWindowSize(ImVec2((float) glWidth * 0.40f, (float) glHeight * 0.65f),
                                         ImGuiCond_Once); // 45% width 70% height

                if (my_window_open) {
                    if (ImGui::Begin("欢迎来到迷你世界", &my_window_open, ImGuiWindowFlags_NoSavedSettings |
                                                                  ImGuiWindowFlags_NoCollapse)) {
                        if (ImGui::GetWindowPos().x<
                                0 || ImGui::GetWindowPos().y<0 || ImGui::GetWindowPos().x>(
                                        glWidth - ImGui::GetWindowWidth()) ||
                                ImGui::GetWindowPos().y>(
                                glHeight - ImGui::GetWindowHeight())) {
                            ImGui::SetWindowPos(ImVec2{glWidth / 3.f, glHeight / 4.f});
                        }

                        if (ImGui::BeginTabBar("Tab", 1)) {
                            if (ImGui::BeginTabItem("透视")) {
                                ImGui::Spacing();

                                ImGui::Checkbox("显示方框", &showBox);
                                ImGui::SameLine();
                                ImGui::RadioButton("信息类型一", &boxType, 1);
                                ImGui::SameLine();
                                ImGui::RadioButton("信息类型二", &boxType, 2);
                                if (ImGui::BeginTable("split", 2)) {
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableNextColumn();
                                    // ImGui::ShowHelpMarker (“全屏防直播，不黑屏”)
                                    ImGui::Checkbox("显示骨骼", &showBone);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("预判方框", &isRunningLoc);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示血量", &showHealth);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示射线", &showLine);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示距离", &showDis);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示准心", &showCrosshair);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示信息", &showInfo);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示雷达", &showRadar);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("手持武器", &showWeapons);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("预警提示", &showGrenade);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("掩体判断", &isVisibility);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("被瞄预警", &isObjAimed);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("击中变色", &isSelectedAim);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示载具", &showVehicle);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("载具信息", &showVehicleInfo);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("载具方框", &showVehicleBox);
                                    ImGui::EndTable();

                                    ImGui::EndTabItem();
                                }
                            }

                            if (ImGui::BeginTabItem("物品")) {
                                ImGui::Spacing();

                                ImGui::Checkbox("显示物品", &showItem);
                                ImGui::SameLine();
                                ImGui::Checkbox("显示盒子", &showChest);
                                ImGui::SameLine();
                                ImGui::Checkbox("显示空投", &showDrop);

                                if (ImGui::BeginTable("split", 2)) {
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示步枪", &showRifle);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示冲锋枪", &showSubmachine);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示狙击枪", &showSniper);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示子弹", &show556);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示倍镜", &showMirror);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示快扩", &showExpansion);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("其他配件", &showOtherParts);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示药品", &showDrug);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示防具", &showArmor);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示投掷物", &showTouzhi);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("显示散弹枪", &showSandan);

                                    ImGui::EndTable();
                                }

                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("自瞄")) {
                                ImGui::Spacing();

                                if (ImGui::BeginTable("split", 2)) {
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("开启自瞄", &my_aimbot);
                                    ImGui::TableNextColumn();
                                    //  ImGui::RadioButton("触摸自瞄", &aimType, 1);
                                    //  ImGui::SameLine();
                                    ImGui::RadioButton("内核自瞄", &aimType, 2);
                                    // ImGui::SameLine();
                                    // ImGui::RadioButton("腰射追踪(危险)", &aimType, 3);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("掩体自瞄", &visibilityAim);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("自瞄线条", &lineAim);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("人机瞄准", &aiAim);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("倒地瞄准", &dieNoAim);
                                    ImGui::TableNextColumn();
                                    //ImGui::Checkbox("触摸位置", &isShowLoc);
                                    // ImGui::TableNextColumn();
                                    // ImGui::Checkbox("屏幕翻转", &isDlip);
                                    //ImGui::TableNextColumn();
                                    ImGui::Checkbox("烟雾不瞄", &isProjSomoke);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("烟雾范围", &isProjSomokeRange);
                                    ImGui::TableNextColumn();
                                    if (aimType == 1)
                                        ImGui::SliderFloat("触摸范围", &SlideRanges, 100.0f, 800.0f);
                                    else
                                        ImGui::SliderFloat("自瞄横扫", &max_shake, 0.0f, 1.5f);
                                    ImGui::TableNextColumn();

                                    ImGui::Checkbox("自瞄范围", &showRange);
                                    ImGui::SameLine();
                                    if (aimRange > 1000.f) {
                                        aimRange = 1000.f;
                                    }
                                    ImGui::SliderFloat("", &aimRange, 0.0f, 1000.0f);
                                    ImGui::TableNextColumn();
                                    ImGui::SliderFloat("烟雾大小", &projSomoke, 15.0f, 1200.0f);

                                    ImGui::TableNextColumn();
                                    ImGui::TableNextColumn();


                                    if (aimType == 1) {

                                        if (ImGui::BeginTable("split", 1)) {
                                            ImGui::TableNextColumn();
                                            const char *touchModeltem[] = {"角度触摸", "平滑触摸"};
                                            ImGui::Combo("触摸方式", &touchMode, touchModeltem,
                                                         IM_ARRAYSIZE(touchModeltem));
                                            ImGui::EndTable();
                                        }

                                        //ImGui::TableNextColumn();
                                        ImGui::SliderFloat("自瞄抖动", &touchJitter, 1.0f, 30.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("预判速度", &touchSpeed, 10.0f, 180.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("横轴位置", &touchLocX, -3200.0f, 3200.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("纵轴位置", &touchLocY, -2000.0f, 2000.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("压枪力度", &touchPress, -100.0f, 100.0f);
                                        ImGui::TableNextColumn();

                                    } else if (aimType == 2) {
                                        // ImGui::TableNextColumn();
                                        ImGui::SliderFloat("自瞄压枪", &aimPress, 0.0f, 60.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("自瞄速度", &aimSpeed, 0.0f, 60.0f);
                                        ImGui::TableNextColumn();
                                        ImGui::SliderFloat("自瞄预判", &aimAnticipation, 0.0f, 60.0f);
                                        ImGui::TableNextColumn();
                                    }

                                    if (ImGui::BeginTable("split", 1)) {
                                        ImGui::TableNextColumn();
                                        const char *aimingState[] = {"开镜自瞄", "开火自瞄", "全部自瞄",
                                                                     "自动模式"};
                                        ImGui::Combo("自瞄模式", &aimedMode, aimingState,
                                                     IM_ARRAYSIZE(aimingState));
                                        ImGui::EndTable();

                                    }
                                    ImGui::TableNextColumn();
                                    if (ImGui::BeginTable("split", 1)) {
                                        ImGui::TableNextColumn();
                                        const char *Choose[] = {"屏幕中心", "自动选择", "距离优先"};
                                        ImGui::Combo("自瞄选择", &aimChoose, Choose,
                                                     IM_ARRAYSIZE(Choose));
                                        ImGui::EndTable();
                                    }
                                    ImGui::TableNextColumn();
                                    if (ImGui::BeginTable("split", 1)) {
                                        ImGui::TableNextColumn();
                                        const char *items[] = {"头部", "胸部", "腰部", "掩体外"};
                                        ImGui::Combo("自瞄位置", &aimLocation, items,
                                                     IM_ARRAYSIZE(items));
                                        ImGui::EndTable();
                                    }
                                    ImGui::TableNextColumn();


                                    ImGui::EndTable();
                                }

                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("设置")) {
                                ImGui::Spacing();

                                if (ImGui::BeginTable("split", 2,
                                                      ImGuiTableFlags_Borders |
                                                      ImGuiTableFlags_Resizable |
                                                      ImGuiTableFlags_Reorderable |
                                                      ImGuiTableFlags_Hideable)) {
                                    ImGui::TableSetupColumn("绘制显示",
                                                            ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
                                    ImGui::TableHeadersRow();
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("图片", &showImg);
                                    ImGui::TableNextColumn();
                                    ImGui::Checkbox("文字", &showText);
                                    ImGui::EndTable();
                                }
                                ImGui::Checkbox("显示距离", &showRadarDis);
                                ImGui::SameLine();
                                ImGui::RadioButton("小地图雷达", &radarType, 1);
                                ImGui::SameLine();
                                ImGui::Checkbox("显示外框", &showRadarBox);
                                ImGui::SameLine();
                                ImGui::RadioButton("360°圆形雷达", &radarType, 2);
                                if (radarType == 1) {
                                    ImGui::SliderFloat("雷达大小", &radarSize, 80.0f, 800.0f);
                                }
                                ImGui::SliderInt("雷达横轴", &radarOffsetX, -1000, 2000);
                                ImGui::SliderInt("雷达纵轴", &radarOffsetY, -1000, 1000);


                                if (ImGui::TreeNode("线条粗细")) {
                                    ImGui::SliderInt("方框粗细", &boxWidth, 1, 15);
                                    ImGui::SliderInt("骨骼粗细", &boneWidth, 1, 15);
                                    ImGui::SliderInt("射线粗细", &lineWidth, 1, 15);

                                    ImGui::TreePop();

                                }
                                if (ImGui::TreeNode("盒子设置")) {
                                    ImGui::Checkbox("显示", &showBoxRange);
                                    ImGui::SliderInt("范围", &boxRange, 1, 1500);
                                    ImGui::RadioButton("盒子全屏显示", &boxShowType, 1);
                                    ImGui::SameLine();
                                    ImGui::RadioButton("盒子范围显示", &boxShowType, 2);
                                    ImGui::SameLine();
                                    ImGui::RadioButton("空投全屏显示", &dropShowType, 1);
                                    ImGui::SameLine();
                                    ImGui::RadioButton("空投范围显示", &dropShowType, 2);

                                    ImGui::TreePop();
                                }


                                if (ImGui::TreeNode("颜色设置")) {
                                    // ColorPicker3 ColorPicker4 ColorEdit3 ColorEdit4
                                    ImGui::ColorEdit4("载具颜色", (float *) &vehicle_color);
                                    ImGui::ColorEdit4("背敌颜色", (float *) &warning_color);
                                    ImGui::ColorEdit4("步枪颜色", (float *) &rifle_color);
                                    ImGui::ColorEdit4("冲锋枪颜色", (float *) &submachine_color);
                                    ImGui::ColorEdit4("狙击枪颜色", (float *) &snipe_color);
                                    ImGui::ColorEdit4("倍镜颜色", (float *) &mirror_color);
                                    ImGui::ColorEdit4("配件颜色", (float *) &expansion_color);
                                    ImGui::ColorEdit4("其他配件颜色", (float *) &otherparts_color);
                                    ImGui::ColorEdit4("药品颜色", (float *) &drug_color);
                                    ImGui::ColorEdit4("防具颜色", (float *) &armor_color);
                                    ImGui::ColorEdit4("投掷物颜色", (float *) &missile_color);
                                    ImGui::ColorEdit4("散弹枪颜色", (float *) &color_shot);
                                    ImGui::ColorEdit4("子弹颜色", (float *) &color_bullet);
                                    ImGui::ColorEdit4("射线颜色", (float *) &line_color);
                                    ImGui::ColorEdit4("方框颜色", (float *) &box_color);
                                    ImGui::ColorEdit4("骨骼顔色", (float *) &bone_color);
                                    ImGui::ColorEdit4("倒地颜色", (float *) &fallen_color);
                                    ImGui::ColorEdit4("人机颜色", (float *) &ai_color);
                                    ImGui::ColorEdit4("掩体颜色", (float *) &visibility_color);
                                    ImGui::ColorEdit4("血条颜色", (float *) &health_color);
                                    ImGui::ColorEdit4("击打颜色", (float *) &selectedAim_color);
                                    ImGui::TreePop();
                                }

                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("页面")) {
                                ImGui::Spacing();
                                const float MIN_SCALE = 0.3f;
                                const float MAX_SCALE = 4.0f;
                                static float window_scale = 1.4f;

                                //ImGui::SliderFloat("绘制缩放", &draw_scale, 1.f, 2.5f);


                                ImGui::ShowStyleSelector("界面顔色##Selector");
                                if (ImGui::DragFloat("界面大小", &window_scale, 0.005f, MIN_SCALE,
                                                     MAX_SCALE, "%.2f",
                                                     ImGuiSliderFlags_AlwaysClamp)) // Scale only this window

                                    ImGui::SetWindowFontScale(window_scale);
                                ImGuiStyle &style = ImGui::GetStyle();
                                ImGui::DragFloat("界面透明", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
                                ImGui::PushID(1);
                                ImGui::PushStyleColor(ImGuiCol_Button,
                                                      (ImVec4) ImColor::HSV(2.f / 7.0f, 0.6f,
                                                                            0.6f));
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.f));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                                      (ImVec4) ImColor::HSV(2.f / 7.0f, 0.7f,
                                                                            0.7f));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                                      (ImVec4) ImColor::HSV(2.f / 7.0f, 0.8f,
                                                                            0.8f));
                                if (ImGui::Button("保存全部设置", ImVec2(600, 80))) {
                                    SaveIni();
                                };
                                ImGui::PopStyleColor(4);
                                ImGui::PopID();
                                ImGui::EndTabItem();
                            }
                            ImGui::EndTabBar();
                        }

                        ImGui::Text("耗时:%.3f ms FPS:%.1f", 1000.0f / ImGui::GetIO().Framerate,
                                    ImGui::GetIO().Framerate);
                    }
                    ImGui::End();
                }
            }
        }
    }
    //登录初始化
    if (!initLogin) {
        initLogin = true;
        //获取当前时间戳
        time_t t;
        time(&t);
        char addr_tmp[32] = "";
        sprintf(addr_tmp, "%lx", my_libUE4 + 0x12589);
        char pid_tmp[32] = "";
        sprintf(pid_tmp, "%d", getpid() + 164513248);

        //获取服务器数据
        string url =
                "http://183.131.79.130:5455/?act=OoOoOoodfgsdnnkdffjsfsdf534ghgf5dsag12OooOoo0O0oOo0O0oO0o0O0o0o0O000&appid=10000&token=" +
                my_token + "&t=" + to_string(t) + "&usertype=" + my_user + "&type=2" +
                "&home=" +
                addr_tmp + "&homeid=" + pid_tmp;
        string strResponse = http_get(url);

        string decryption3, decryption4;
        char *key = "{gjgsoagjsoajga==}";
        char *key2 = "{sdjfldsjglj==}";
        char *key3 = "{idjgldsjglsj==}";
        //开始解密数据
        OoOoOooOoOOo(key3, decryption4, decryption4, key, key2, (char *) strResponse.c_str(), '{',
                     '}',
                     'o', (char *) decryption.c_str(), decryption4, decryption3, decryption3);
        //登录失败
        if (strstr(decryption.c_str(), "123456789o") || strstr(decryption.c_str(), "4848484o")) {
            LOGD("登录失败");
            loginCode = 1001;
            goto _break;
            //return;
        }
        //账号被禁用
        if (strstr(decryption.c_str(), "46461919o")) {
            LOGD("账号被禁用");
            loginCode = 1002;
            goto _break;
            //return;
        }
        //服务器返回id判断
        string OoOOoOoOiOOoOo = strstrstr(decryption.c_str(), "OoOOoOoOiOOoOo:", "\n", false);
        string OoOOoOoOiOOoo = "183.131.79.130";
        if (!strstr(OoOOoOoOiOOoOo.c_str(), OoOOoOoOiOOoo.c_str())) {
            LOGD("ip错误");
            loginCode = 1003;
            //return;
            goto _break;
        }
        int va = atoi(strstrstr(decryption.c_str(), "Oo0o0O0o:", "\n", false));
        //版本有效期
        int timeTmp = atoi(strstrstr(decryption.c_str(), "time:", "\n", false));
        time_t t1;
        time(&t1);
        if ((int) t1 > app_effective || (int) t1 > timeTmp) {
            LOGD("程序过期");
            loginCode = 1004;
            //return;
            goto _break;
        }
        //数据获取成功的校验
        if (va != 589478912) {
            LOGD("校验错误");
            loginCode = 1005;
            //return;
            goto _break;
        } else {
            LOGD("登录成功");
            loginCode = 1007;
        }
        //判断数据是否请求超时
        int timeStamp = atoi(strstrstr(decryption.c_str(), "timeStamp:", "\n", false));
        if (timeStamp - t1 > 100) {
            LOGD("请求超时：%s", strResponse.c_str());
            loginCode = 1006;
            //return;
            goto _break;
        }
    }
    _break:
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return orig_eglSwapBuffers(dpy, surface);
}



/**
 * 得到剪切板
 * @return
 */
std::string getClipboardText() {
    if (!g_App)
        return "";

    auto activity = g_App->activity;
    if (!activity)
        return "";

    auto vm = activity->vm;
    if (!vm)
        return "";

    auto object = activity->clazz;
    if (!object)
        return "";

    std::string result;

    JNIEnv *env;
    vm->AttachCurrentThread(&env, 0);
    {
        auto ContextClass = env->FindClass("android/content/Context");
        auto getSystemServiceMethod = env->GetMethodID(ContextClass, "getSystemService",
                                                       "(Ljava/lang/String;)Ljava/lang/Object;");

        auto str = env->NewStringUTF("clipboard");
        auto clipboardManager = env->CallObjectMethod(object, getSystemServiceMethod, str);
        env->DeleteLocalRef(str);

        auto ClipboardManagerClass = env->FindClass("android/content/ClipboardManager");
        auto getText = env->GetMethodID(ClipboardManagerClass, "getText",
                                        "()Ljava/lang/CharSequence;");

        auto CharSequenceClass = env->FindClass("java/lang/CharSequence");
        auto toStringMethod = env->GetMethodID(CharSequenceClass, "toString",
                                               "()Ljava/lang/String;");

        auto text = env->CallObjectMethod(clipboardManager, getText);
        if (text) {
            str = (jstring) env->CallObjectMethod(text, toStringMethod);
            result = env->GetStringUTFChars(str, 0);
            env->DeleteLocalRef(str);
            env->DeleteLocalRef(text);
        }
        env->DeleteLocalRef(CharSequenceClass);
        env->DeleteLocalRef(ClipboardManagerClass);
        env->DeleteLocalRef(clipboardManager);
        env->DeleteLocalRef(ContextClass);
    }
    vm->DetachCurrentThread();

    return result;
}

/**
 * 内存映射得到登录数据
 * @return
 */
static bool initMemoryShare() {
    InitData *initData;
    int mmapFd = (int) raw_syscall(__NR_openat, AT_FDCWD, "/storage/emulated/0/imgui_init/init",
                                   O_RDWR, 0660);
    if (mmapFd < 1) { return false; }
    //创建映射
    initData = (InitData *) mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, mmapFd, 0);
    if (initData == nullptr) { return false; }
    close(mmapFd);
    GNativeAndroidApp = initData->init_data;
    my_token = initData->sToken + initData->eToken;
    initState = initData->init_state;
    my_user = initData->user;
    munmap(initData, 1024);
    system("rm -r /storage/emulated/0/imgui_init/init");
    return true;
}

/**
 * 主线程
 * @return
 */
void *main_thread(void *) {
    prctl(PR_SET_NAME, "main_thread");
    //#define GNativeAndroidApp 0xA211AD8 //120D;119D;50D;1024D:: 改50 偏-20 32位特征码
    //#define GNativeAndroidApp 0x7337478 //16384D;16384D;1D;4096D;2D::17  改16384 偏-40 64位特征码
    //#define GNativeAndroidApp 0xA62D228 //国服 174,248,488
    // Java_com_epicgames_ue4_GameActivity_nativeVirtualKeyboardVisible	跟进 STR指令 XZR, [X19,#(off_A62D310 - 0xA62D2B0)] GNativeAndroidApp 在0xA62D2B0 上面off_xxx - unk_xxx就是了
    if (!initMemoryShare()) {
        LOGD("映射失败返回游戏");
        return nullptr;
    }
    if (strstr(initState.c_str(), "init_true") == nullptr) {
        LOGD("登录未完成,返回游戏");
        return nullptr;
    }
    sleep(5);
    while (!my_libUE4) {
        LOGD("!my_libUE4");
        my_libUE4 = getModule("libUE4.so", true);
    }
    while (!g_App) {
        LOGD("!g_App");
        g_App = *(android_app **) (my_libUE4 + atoi(GNativeAndroidApp.c_str()) + 45894);
        sleep(1);
    }
/*
      char a[32];
      memoryRead(my_libUE4 + atoi(GNativeAndroidApp.c_str()) + 45894, a, sizeof(a));
      LOGD("读取字节码:%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %lx - %lx",
           a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],a[16],
           a[17], a[18], a[19], a[20], a[21], a[22], a[23], a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31]
           ,FindPattern("libUE4.so","120 0 0 0 64 ? ? 36 120 0 0 0 48 ? ? ? 120 0 0 0 128 ? ? ? 120 0 0 0"),my_libUE4 + atoi(GNativeAndroidApp.c_str()) + 45894);
      //  213, 36, 224, 231(d5 24 e0 e7)   01 ,32, 160 ,227(01 20 a0 e3)

      //*(reinterpret_cast<int *>(libUE4 + 0x03E53A40)) = 0x01;
      //*(reinterpret_cast<int *>(libUE4 + 0x03E53A40 + 4)) = 0x20;
      //*(reinterpret_cast<int *>(libUE4 + 0x03E53A40 + 8)) = 0xa0;
      //*(reinterpret_cast<int *>(libUE4 + 0x03E53A40 + 12)) = 0xe3;

      vm_readv(libUE4 + 0x03E53A40, a, sizeof(a));

      LOGD("修改后:%d %d %d %d", a[0], a[1], a[2], a[3]);

      */
    void *egl = dlopen_ex("libEGL.so", 4);
    while (!egl) {
        LOGD("!egl");
        egl = dlopen_ex("libEGL.so", 4);
        sleep(1);
    }
    void *addr = dlsym_ex(egl, "eglSwapBuffers");
    DobbyHook((void *) addr, (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
    //MSHookFunction((void *) addr, (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
    //HOOK(addr, _eglSwapBuffers, &orig_eglSwapBuffers);
    //WInlineHookFunction((void *) addr, (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
    //hook surfaceflinger进程的eglSwapBuffers方式实现绘制
    dlclose_ex(egl);
    //LOGD("剪切板:%s",getClipboardText().c_str());
    return nullptr;
}

//设置访问权限 宽容模式
//setenforce 0
//setprop wrap.com.tencent.tmgp.pubgmhd LD_PRELOAD=so路径

//__attribute__((constructor)) void init() {

//}

/*
__attribute__((constructor))void init() {
    LOGD("LD_PRELOAD");

    pthread_t t;
    pthread_create(&t, nullptr, main_thread, nullptr);
}*/