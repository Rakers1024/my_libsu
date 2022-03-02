#ifndef STRUCTSCOMM_H
#define STRUCTSCOMM_H

#include <string>
#include <string.h>
#include <imgui.h>
#include <android/native_activity.h>
#include <android/configuration.h>
typedef char UTF8;
typedef unsigned short UTF16;
using namespace std;

#define MaxPlayerCount 50
#define MaxItemsCount 30
#define MaxVehicleCount 30
#define MaxGrenadeCount 30
#define MaxAirBoxCount 30
#define MaxActorCount 1024
#define MaxValidateCount 1024
struct HashMap {
    uintptr_t addr;
    size_t size;
};

struct Vec4 {
    float X, Y, Z, W;
};

struct Vector2A {
    float X;
    float Y;

    Vector2A() {
        this->X = 0;
        this->Y = 0;
    }

    Vector2A(float x, float y) {
        this->X = x;
        this->Y = y;
    }
};

static struct Color {
    ImVec4 Red = {255 / 255.f, 0 / 255.f, 0 / 255.f, 255 / 255.f};
    ImVec4 Red_ = {255 / 255.f, 0 / 255.f, 0 / 255.f, 50 / 255.f};
    ImVec4 Green = {0 / 255.f, 255 / 255.f, 0 / 255.f, 255 / 255.f};
    ImVec4 Green_ = {0 / 255.f, 255 / 255.f, 0 / 255.f, 50 / 255.f};
    ImVec4 White = {1.0, 1.0, 1.0, 1.0};
    ImVec4 White_ = {255 / 255.f, 255 / 255.f, 255 / 255.f, 180.f / 255.f};
    ImVec4 Black = {0 / 255.f, 0 / 255.f, 0 / 255.f, 255.f / 255.f};
    ImVec4 Yellow = {255 / 255.f, 255 / 255.f, 0, 255 / 255.f};
} Color;


//屏幕坐标结构
typedef struct Rect {
    float x;
    float y;
    float w;
    float h;
};

struct Vector3A {
    float X;
    float Y;
    float Z;

    Vector3A() {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    Vector3A(float x, float y, float z) {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

};

struct Vector4A {
    float x;
    float y;
    float z;
    float w;

    Vector4A() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 0;
    }


    Vector4A(float x, float y, float h, float w) {
        this->x = x;
        this->y = y;
        this->z = h;
        this->w = w;
    }
};


struct Request {
    int Mode;
    int ScreenWidth;
    int ScreenHeight;
    bool isSpeed;
    bool isBone;
    bool isItems;
    bool isVehicle;
    bool isGrenade;
    bool isAirBox;
};


struct D3DMatrix {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};


struct Draw3DBox {
    Vector2A v1;
    Vector2A v2;
    Vector2A v3;
    Vector2A v4;
    Vector2A v5;
    Vector2A v6;
    Vector2A v7;
    Vector2A v8;
};

struct BoneData {
    Vector3A Head;
    Vector3A vNeck;
    Vector3A Chest;
    Vector3A Pelvis;
    Vector3A Left_Shoulder;
    Vector3A Right_Shoulder;
    Vector3A Left_Elbow;
    Vector3A Right_Elbow;
    Vector3A Left_Wrist;
    Vector3A Right_Wrist;
    Vector3A Left_Thigh;
    Vector3A Right_Thigh;
    Vector3A Left_Knee;
    Vector3A Right_Knee;
    Vector3A Left_Ankle;
    Vector3A Right_Ankle;
};
struct WeaponData {
    bool isWeapon = false;
    int id;
    int ammo;
    int clip;
};

typedef struct {
    float x;
    float y;
    float h;
    int d;
} Vector4Screen;
struct android_poll_source {
    int32_t id;
    struct android_app *app;
    void (*process)(struct android_app *app, struct android_poll_source *source);
};


struct android_app {
    // The application can place a pointer to its own state object
    // here if it likes.
    void *userData;

    // Fill this in with the function to process main app commands (APP_CMD_*)
    void (*onAppCmd)(struct android_app *app, int32_t cmd);

    // Fill this in with the function to process input events.  At this point
    // the event has already been pre-dispatched, and it will be finished upon
    // return.  Return 1 if you have handled the event, 0 for any default
    // dispatching.
    int32_t (*onInputEvent)(struct android_app *app, AInputEvent *event);

    // The ANativeActivity object instance that this app is running in.
    ANativeActivity *activity;

    // The current configuration the app is running in.
    AConfiguration *config;

    // This is the last instance's saved state, as provided at creation time.
    // It is NULL if there was no state.  You can use this as you need; the
    // memory will remain around until you call android_app_exec_cmd() for
    // APP_CMD_RESUME, at which point it will be freed and savedState set to NULL.
    // These variables should only be changed when processing a APP_CMD_SAVE_STATE,
    // at which point they will be initialized to NULL and you can malloc your
    // state and place the information here.  In that case the memory will be
    // freed for you later.
    void *savedState;
    size_t savedStateSize;

    // The ALooper associated with the app's thread.
    ALooper *looper;

    // When non-NULL, this is the input queue from which the app will
    // receive user input events.
    AInputQueue *inputQueue;

    // When non-NULL, this is the window surface that the app can draw in.
    ANativeWindow *window;

    // Current content rectangle of the window; this is the area where the
    // window's content should be placed to be seen by the user.
    ARect contentRect;

    // Current state of the app's activity.  May be either APP_CMD_START,
    // APP_CMD_RESUME, APP_CMD_PAUSE, or APP_CMD_STOP; see below.
    int activityState;

    // This is non-zero when the application's NativeActivity is being
    // destroyed and waiting for the app thread to complete.
    int destroyRequested;

    // -------------------------------------------------
    // Below are "private" implementation of the glue code.

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int msgread;
    int msgwrite;

    pthread_t thread;

    struct android_poll_source cmdPollSource;
    struct android_poll_source inputPollSource;

    int running;
    int stateSaved;
    int destroyed;
    int redrawNeeded;
    AInputQueue *pendingInputQueue;
    ANativeWindow *pendingWindow;
    ARect pendingContentRect;
};

struct ItemData {
    float Distance;
    Vector4Screen Location;
    int ItemNum;
    float ItemHP;
    float ItemFuel;
};
struct TextureInfo {
    ImTextureID textureId;
    int x;
    int y;
    int w;
    int h;
};
struct PlayerData {
    bool isAI;
    bool isRat;
    int TeamID;
    float Health;
    float Angle;
    int State;
    bool isVisibility;
    float Distance;
    UTF8 PlayerName[64];
    float HeadSize;
    BoneData mBoneData;
    Vector4Screen Location;
    Vector2A RadarLocation;
    WeaponData Weapon;
    Vector2A RotationAngle;
    Vector4Screen runningLoc;
    Draw3DBox draw3DBox;
    Vector2A objAim;
};


struct FMatrix {
    float M[4][4];
};

struct Quat {
    float X;
    float Y;
    float Z;
    float W;
};
struct InitData {
    string sToken;
    string eToken;
    string init_data;
    string init_state;
    string user;
};


struct FTransform {
    Quat Rotation;
    Vector3A Translation;
    float chunk;
    Vector3A Scale3D;
};

struct Update {
    bool isRat;
    bool isAI;
    int TeamID;
    uintptr_t actorBases;
    UTF8 PlayerName[64];
    uintptr_t plater_mesh;
    uintptr_t plater_mesh_;
    uintptr_t player_controllers;
    uintptr_t plater_bone;
    uintptr_t RootCompont;//坐标指针
    //物品
    string ItemName;//类名
    string ClassPath; //类
    uintptr_t vehicleCommonComponent;//车辆配置
};


typedef struct {
    Vector4A Rotation;
    Vector3A Translation;
    Vector3A Scale3D;
} _FTransform;

#endif