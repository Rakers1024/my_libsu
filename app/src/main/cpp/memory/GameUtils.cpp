//
// Created by admin on 2022/2/19.
//

#include <cstdint>
#include <string>
#include <EGL/egl.h>
#include <Draw.h>
#include <linux/prctl.h>
#include "GameUtils.h"
#include "Offsets.h"
#include "MemoryUtils.h"
#include "Structs.h"
#include "../include/Engine.h"
#include <sys/prctl.h>

std::string getUEString(uintptr_t address);

std::string GetFNameFromID(unsigned int index, uintptr_t GnameAddress);

std::string getName(uintptr_t actor, uintptr_t GnameAddress);

void getClassName(uintptr_t obj, char *name, uintptr_t GnameAddress);

std::string GetFNameFromID(unsigned int index, uintptr_t GnameAddress);

std::string getName(uintptr_t actor, uintptr_t GnameAddress);

void getClassName(uintptr_t obj, char *name, uintptr_t GnameAddress);

std::string getUEString(uintptr_t address);

float getDistance(float zx, float zy, float zz, float dx, float dy, float dz);

bool isContain(std::string str, const char *check);

Vector2A rotateCoord(float angle, float objectRadar_x, float objectRadar_y);

struct Vector2A World2ScreenMain(struct D3DMatrix viewMatrix, struct Vector3A pos);

void
ToMatrixWithScale(Vector4A Rotation, Vector3A Translation, Vector3A Scale3D, D3DMatrix &result);

bool worldToScreen(FVector *WorldLocationPtr, FVector *pOut);

void getPlayerName(UTF8 *buf, long int nameOffset);

/*
std::string getUEString(uintptr_t address) {
    char ClassName[32] = "";
    memoryRead(address, ClassName, 32);
    std::string uestring(ClassName);
    return uestring;
}
std::string GetFNameFromID(unsigned int index, uintptr_t GnameAddress) {
    uintptr_t FNameEntryArr = getA(GnameAddress + ((index / 0x4000) * Offset::PointerSize));
    uintptr_t FNameEntry = getA(FNameEntryArr + ((index % 0x4000) * Offset::PointerSize));
    return getUEString(FNameEntry + Offset::FNameEntryToNameString);

}
std::string getName(uintptr_t actor, uintptr_t GnameAddress) {
    return GetFNameFromID(getI(actor + Offset::UObjectToInternalIndex), GnameAddress);
}
*/

string GetFNameFromID(unsigned int index, uintptr_t GnameAddress) {
    uintptr_t FNameEntryArr = getA(GnameAddress + ((index / 0x4000) * Offset::PointerSize));
    uintptr_t FNameEntry = getA(FNameEntryArr + ((index % 0x4000) * Offset::PointerSize));
    char name[128] = {};
    memoryRead(FNameEntry + Offset::FNameEntryToNameString, name, 100);
    string str = string(name);
    str.shrink_to_fit();
    return str;
}

uintptr_t getClass(uintptr_t object) {//UClass*
    return getA(object + Offset::UObjectToClassPrivate);
}

int getNameID(uintptr_t object) {
    return getI(object + Offset::UObjectToInternalIndex);
}

string getName(uintptr_t object, uintptr_t GnameAddress) {
    return GetFNameFromID(getNameID(object), GnameAddress);
}

uintptr_t getSuperClass(uintptr_t structz) {
    return getA(structz + Offset::UStructToSuperStruct);
}

string getClassPath(uintptr_t object, uintptr_t GnameAddress) {
    uintptr_t clazz = getClass(object);
    string classname = getName(clazz, GnameAddress);

    uintptr_t superclass = getSuperClass(clazz);
    while (superclass) {
        classname += ".";
        classname += getName(superclass, GnameAddress);
        superclass = getSuperClass(superclass);
    }
    return classname;
}

//android_app *g_App = 0;

void getPlayerName(UTF8 *buf, long int nameOffset) {
    UTF16 buf16[16] = {0};
    memoryRead(nameOffset, buf16, 28);
    UTF16 *pTempUTF16 = buf16;
    UTF8 *pTempUTF8 = buf;
    UTF8 *pUTF8End = pTempUTF8 + 32;
    while (pTempUTF16 < pTempUTF16 + 28) {
        if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End) {
            *pTempUTF8++ = (UTF8) *pTempUTF16;
        } else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End) {
            *pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        } else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End) {
            *pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
            *pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        } else {
            break;
        }
        pTempUTF16++;
    }
}

struct D3DMatrix
ToMatrixWithScale(struct Vector3A translation, struct Vector3A scale, struct Vec4 rot) {
    struct D3DMatrix m;
    m._41 = translation.X;
    m._42 = translation.Y;
    m._43 = translation.Z;

    float x2 = rot.X + rot.X;
    float y2 = rot.Y + rot.Y;
    float z2 = rot.Z + rot.Z;


    float xx2 = rot.X * x2;
    float yy2 = rot.Y * y2;
    float zz2 = rot.Z * z2;

    m._11 = (1.0f - (yy2 + zz2)) * scale.X;
    m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
    m._33 = (1.0f - (xx2 + yy2)) * scale.Z;


    float yz2 = rot.Y * z2;
    float wx2 = rot.W * x2;

    m._32 = (yz2 - wx2) * scale.Z;
    m._23 = (yz2 + wx2) * scale.Y;

    float xy2 = rot.X * y2;
    float wz2 = rot.W * z2;

    m._21 = (xy2 - wz2) * scale.Y;
    m._12 = (xy2 + wz2) * scale.X;


    float xz2 = rot.X * z2;
    float wy2 = rot.W * y2;

    m._31 = (xz2 + wy2) * scale.Z;
    m._13 = (xz2 - wy2) * scale.X;

    m._14 = 0.0f;
    m._24 = 0.0f;
    m._34 = 0.0f;
    m._44 = 1.0f;

    return m;
}

//???????????????
static float tm = 255 / 255.f;

static ImVec4 arr[] = {{144 / 255.f, 238 / 255.f, 144 / 255.f, tm},
                       {135 / 255.f, 206 / 255.f, 255 / 255.f, tm},
                       {255 / 255.f, 0 / 255.f,   0 / 255.f,   tm},
                       {0 / 255.f,   255 / 255.f, 0 / 255.f,   tm},
                       {0 / 255.f,   255 / 255.f, 127 / 255.f, tm},
                       {255 / 255.f, 182 / 255.f, 193 / 255.f, tm},
                       {218 / 255.f, 112 / 255.f, 214 / 255.f, tm},
                       {248 / 255.f, 248 / 255.f, 255 / 255.f, tm},
                       {0 / 255.f,   255 / 255.f, 255 / 255.f, tm},
                       {255 / 255.f, 165 / 255.f, 0 / 255.f,   tm},
                       {153 / 255.f, 204 / 255.f, 255 / 255.f, tm},
                       {204 / 255.f, 255 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 255 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 153 / 255.f, 153 / 255.f, tm},
                       {153 / 255.f, 153 / 255.f, 204 / 255.f, tm},
                       {204 / 255.f, 204 / 255.f, 204 / 255.f, tm},
                       {102 / 255.f, 204 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 102 / 255.f, 0 / 255.f,   tm},
                       {102 / 255.f, 204 / 255.f, 204 / 255.f, tm},
                       {153 / 255.f, 204 / 255.f, 255 / 255.f, tm}
};
static int length = sizeof(arr) / 20;

struct Vector3A mat2Cord(struct D3DMatrix pM1, struct D3DMatrix pM2) {
    struct Vector3A pOut;
    pOut.X = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut.Y = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut.Z = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;

    return pOut;
}


/* ??????3D???????????? */
float getDistance(float zx, float zy, float zz, float dx, float dy, float dz) {
    float x, y, z;
    x = zx - dx;
    y = zy - dy;
    z = zz - dz;
    return (float) (sqrt(x * x + y * y + z * z));
}


FMatrix MatrixMulti(FMatrix m1, FMatrix m2) {
    FMatrix matrix = FMatrix();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
            }
        }
    }
    return matrix;
}

struct Vector2A World2ScreenMain(struct D3DMatrix viewMatrix, struct Vector3A pos) {
    struct Vector2A screen;
    float screenW = (viewMatrix._14 * pos.X) + (viewMatrix._24 * pos.Y) + (viewMatrix._34 * pos.Z) +
                    viewMatrix._44;
    float screenX = (viewMatrix._11 * pos.X) + (viewMatrix._21 * pos.Y) + (viewMatrix._31 * pos.Z) +
                    viewMatrix._41;
    float screenY = (viewMatrix._12 * pos.X) + (viewMatrix._22 * pos.Y) + (viewMatrix._32 * pos.Z) +
                    viewMatrix._42;
    screen.Y = (py) - (py) * screenY / screenW;
    screen.X = (px) + (px) * screenX / screenW;
    return screen;
}


struct Vec4 rot;
struct Vector3A scale, tran;

struct D3DMatrix getOMatrix(uintptr_t boneAddr) {
    float oMat[11];
    memoryRead(boneAddr, &oMat, sizeof(oMat));
    rot.X = oMat[0];
    rot.Y = oMat[1];
    rot.Z = oMat[2];
    rot.W = oMat[3];

    tran.X = oMat[4];
    tran.Y = oMat[5];
    tran.Z = oMat[6];

    scale.X = oMat[8];
    scale.Y = oMat[9];
    scale.Z = oMat[10];

    return ToMatrixWithScale(tran, scale, rot);
}


//????????????????????????
float get2dDistance(float x, float y, float x1, float y1) {
    float xx1 = x - x1;
    float yy2 = y - y1;
    // ????????????
    return sqrt(xx1 * xx1 + yy2 * yy2);
}

//??????????????????
float getD3Distance(Vector3A Self, Vector3A Object) {
    float x, y, z;
    x = Self.X - Object.X;
    y = Self.Y - Object.Y;
    z = Self.Z - Object.Z;
    // ????????????
    return (float) (sqrt(x * x + y * y + z * z));
}

FTransform getBone(uintptr_t addr) {
    FTransform transform;
    memoryRead(addr, &transform, 4 * 11);
    return transform;
}


bool isContain(std::string str, const char *check) {
    size_t found = str.find(check);
    return (found != std::string::npos);
}

bool isEqual(std::string s1, const char *check) {
    std::string s2(check);
    return (s1 == s2);
}


// ??????????????????
Vector2A rotateCoord(float angle, float objectRadar_x, float objectRadar_y) {
    Vector2A radarCoordinate;
    float s = sin(angle * PI / 180);
    float c = cos(angle * PI / 180);
    radarCoordinate.X = objectRadar_x * c + objectRadar_y * s;
    radarCoordinate.Y = -objectRadar_x * s + objectRadar_y * c;
    return radarCoordinate;
}


float big(float a, float b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

float small(float a, float b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

float angle(float a, float b) {
    float c = a - b;
    float d = small(a, b) + 360 - big(a, b);
    if (fabs(c) > d) {
        if (small(a, b) == a) {
            d = -d;
        }
        return d;
    } else { return -c; }
}

float smallangle(float a, float b, float c) {
    float d = angle(a, b);
    if (fabs(d) > c) {
        if (d < 0) {
            d = -c;
        } else {
            d = c;
        }
    };
    float e = a + d;
    if (e > 360) {
        e = e - 360;
    } else if (e < 0) {
        e = 360 + e;
    };
    return e;
}


/*
 * ????????????
 */
void getBoneLocation(uintptr_t mesh, Vector3A &result, uintptr_t bone) {
    struct D3DMatrix baseMatrix = getOMatrix(mesh);
    result = mat2Cord(getOMatrix(bone), baseMatrix);
}


float caculDis(Vector3A vPos1, Vector3A vPos2) {
    return sqrt(pow(vPos1.X - vPos2.X, 2) + pow(vPos1.Y - vPos2.Y, 2) + pow(vPos1.Z - vPos2.Z, 2)) /
           100.0f;
}

void D3DXMatrixMultiply(D3DMatrix pM1, D3DMatrix pM2, D3DMatrix &pOut) {
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
}

void
ToMatrixWithScale(Vector4A Rotation, Vector3A Translation, Vector3A Scale3D, D3DMatrix &result) {
    float x2, y2, z2, xx2, yy2, zz2, yz2, wx2, xy2, wz2, xz2, wy2;

    result._41 = Translation.X;
    result._42 = Translation.Y;
    result._43 = Translation.Z;

    x2 = Rotation.x + Rotation.x;
    y2 = Rotation.y + Rotation.y;
    z2 = Rotation.z + Rotation.z;

    xx2 = Rotation.x * x2;
    yy2 = Rotation.y * y2;
    zz2 = Rotation.z * z2;
    result._11 = (1 - (yy2 + zz2)) * Scale3D.X;
    result._22 = (1 - (xx2 + zz2)) * Scale3D.Y;
    result._33 = (1 - (xx2 + yy2)) * Scale3D.Z;


    yz2 = Rotation.y * z2;
    wx2 = Rotation.w * x2;
    result._32 = (yz2 - wx2) * Scale3D.Z;
    result._23 = (yz2 + wx2) * Scale3D.Y;


    xy2 = Rotation.x * y2;
    wz2 = Rotation.w * z2;
    result._21 = (xy2 - wz2) * Scale3D.Y;
    result._12 = (xy2 + wz2) * Scale3D.X;


    xz2 = Rotation.x * z2;
    wy2 = Rotation.w * y2;
    result._31 = (xz2 + wy2) * Scale3D.Z;
    result._13 = (xz2 - wy2) * Scale3D.X;

    result._14 = 0;
    result._24 = 0;
    result._34 = 0;
    result._44 = 1;
}

void ReadFTransform(uintptr_t addr, _FTransform &ans) {
    memoryRead(addr, &ans, sizeof(_FTransform));
    ans.Scale3D.X = ans.Scale3D.Y;
}


bool worldToScreen(Vector3A *WorldLocationPtr, Vector4Screen *pOut) {
    FVector world = FVector(WorldLocationPtr->X, WorldLocationPtr->Y, WorldLocationPtr->Z);
    FVector screen;
    bool isShow = CameraManager::WorldToScreen(world, &screen);
    pOut->x = screen.x;
    pOut->y = screen.y;

    FVector world2 = FVector(WorldLocationPtr->X, WorldLocationPtr->Y,
                             WorldLocationPtr->Z + 100.0f);
    FVector screen2;
    CameraManager::WorldToScreen(world2, &screen2);

    Vector3A location;
    memoryRead(playerCameraManagerBase + Offset::dw_CameraCacheEntry + Offset::dw_MinimalViewInfo +
               Offset::dw_Location, &location, sizeof(location));
    pOut->d = (int) caculDis(*WorldLocationPtr, location);
    pOut->h = screen.y - screen2.y;
    return isShow;
}

bool worldToScreen(Vector3A *WorldLocationPtr, Vector3A *pOut) {
    FVector world = FVector(WorldLocationPtr->X, WorldLocationPtr->Y, WorldLocationPtr->Z);
    FVector screen;
    bool isShow = CameraManager::WorldToScreen(world, &screen);
    pOut->X = screen.x;
    pOut->Y = screen.y;
    return isShow;
}


bool worldToScreen(FVector *WorldLocationPtr, FVector *pOut) {
    FVector world = FVector(WorldLocationPtr->x, WorldLocationPtr->y, WorldLocationPtr->z);
    FVector screen;
    bool isShow = CameraManager::WorldToScreen(world, &screen);
    pOut->x = screen.x;
    pOut->y = screen.y;
    return isShow;
}

double ArcToAngle(double angle) {
    return angle * (double) 57.29577951308;
}

Vector2A CalcAngle(Vector3A D, Vector3A W) {
    float x = W.X - D.X;
    float y = W.Y - D.Y;
    float z = W.Z - D.Z;
    FVector angle = FVector(ArcToAngle(atan2(y, x)), ArcToAngle(atan2(z, sqrt(x * x + y * y))),
                            0.0f);
    Vector2A angle2;
    angle2.X = angle.x;
    angle2.Y = angle.y;
    return angle2;
}

/**
 * ?????? ?????????2.3.
 *
 */
WeaponData getPlayerWeapon(uintptr_t base) {
    WeaponData p;
    int holdingState = getI(base + Offset::holdingStateOffset); //????????????
    uintptr_t my_weapon = getA(base + Offset::weaponOffset);
    if (holdingState == 1 || holdingState == 2 || holdingState == 3) {
        // if(mode == 2){
        //      p.id = getI(getA(getA(base + Offset::weaponOffset) + Offset::gunOffset) +Offset::weaponsIDOffset);
        // }else{
        p.id = getI(my_weapon + Offset::weaponIDOffset); //ID

        //  }
        p.ammo = getI(my_weapon + Offset::weaponAmmoOffset); //????????????
        p.clip = getI(my_weapon + Offset::weaponClipOffset); //??????
        p.isWeapon = true;
    } else
        p.isWeapon = false;
    return p;
}

void Box3D(Vector3A origin, Vector3A extends) {

    origin.X -= extends.X / 2.f;
    origin.Y -= extends.Y / 2.f;
    origin.Z -= extends.Z / 2.f;

    // bottom plane
    Vector3A one = origin;
    Vector3A two = origin;
    two.X += extends.X;
    Vector3A three = origin;
    three.X += extends.X;
    three.Y += extends.Y;
    Vector3A four = origin;
    four.Y += extends.Y;

    Vector3A five = one;
    five.Z += extends.Z;
    Vector3A six = two;
    six.Z += extends.Z;
    Vector3A seven = three;
    seven.Z += extends.Z;
    Vector3A eight = four;
    eight.Z += extends.Z;

    Vector3A s1, s2, s3, s4, s5, s6, s7, s8;


    if (!worldToScreen(&one, &s1) || !worldToScreen(&two, &s2) || !worldToScreen(&three, &s3) ||
        !worldToScreen(&four, &s4))
        return;
    if (!worldToScreen(&five, &s5) || !worldToScreen(&six, &s6) || !worldToScreen(&seven, &s7) ||
        !worldToScreen(&eight, &s8))
        return;
    Draw3DBox draw3DBox;
    draw3DBox.v1.X = s1.X;
    draw3DBox.v1.Y = s1.Y;
    draw3DBox.v2.X = s2.X;
    draw3DBox.v2.Y = s2.Y;
    draw3DBox.v3.X = s3.X;
    draw3DBox.v3.Y = s3.Y;
    draw3DBox.v4.X = s4.X;
    draw3DBox.v4.Y = s4.Y;
    draw3DBox.v5.X = s5.X;
    draw3DBox.v5.Y = s5.Y;
    draw3DBox.v6.X = s6.X;
    draw3DBox.v6.Y = s6.Y;
    draw3DBox.v7.X = s7.X;
    draw3DBox.v7.Y = s7.Y;
    draw3DBox.v8.X = s8.X;
    draw3DBox.v8.Y = s8.Y;

    DrawBoxLine(draw3DBox.v1, draw3DBox.v2, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v2, draw3DBox.v3, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v3, draw3DBox.v4, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v4, draw3DBox.v1, box_color, boxWidth);

    DrawBoxLine(draw3DBox.v5, draw3DBox.v6, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v6, draw3DBox.v7, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v7, draw3DBox.v8, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v8, draw3DBox.v5, box_color, boxWidth);

    DrawBoxLine(draw3DBox.v1, draw3DBox.v5, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v2, draw3DBox.v6, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v3, draw3DBox.v7, box_color, boxWidth);
    DrawBoxLine(draw3DBox.v4, draw3DBox.v8, box_color, boxWidth);
}

bool getisDie(int state, float health) {
    return state == 524288 || state == 524289 || state == 524305 || health == 0;
}

Vector3A getBoneLoc(uintptr_t mesh, int BoneIndex);

bool isVisiblePoint(Vector3A point);

bool isSafeAddr(Vector3A point);

bool WorldToScreenPoint(Vector3A Enemy, Vector3A &_Screen) {
    FVector world = FVector(Enemy.X, Enemy.Y, Enemy.Z);
    FVector screen;
    bool isShow = CameraManager::WorldToScreen(world, &screen);
    if (isSafeAddr({Enemy.X, Enemy.Y, Enemy.Z}))return false;
    _Screen.X = screen.x;
    _Screen.Y = screen.y;
    if (isShow && isVisibility) {
        isVisiblePoint(Enemy) ? _Screen.Z = 1 : _Screen.Z = 0;
    }
    return isShow;
}

void DrawBone(uintptr_t mesh, BoneData *skeleton) {

    Vector3A vHead, vChest, vNeck, vPelvis, vShoulderL, vShoulderR, vElbowL, vElbowR, vWristL, vWristR, vLegL, vLegR, vKneeL, vKneeR, vAnkleL, vAnkleR;
    vHead = getBoneLoc(mesh, 6);
    vNeck = getBoneLoc(mesh, 5);
    vChest = getBoneLoc(mesh, 4);
    vPelvis = getBoneLoc(mesh, 1);
    vShoulderL = getBoneLoc(mesh, 11);
    vShoulderR = getBoneLoc(mesh, 32);
    vElbowL = getBoneLoc(mesh, 12);
    vElbowR = getBoneLoc(mesh, 33);
    vWristL = getBoneLoc(mesh, 63);
    vWristR = getBoneLoc(mesh, 62);
    vLegL = getBoneLoc(mesh, 52);
    vLegR = getBoneLoc(mesh, 56);
    vKneeL = getBoneLoc(mesh, 53);
    vKneeR = getBoneLoc(mesh, 57);
    vAnkleL = getBoneLoc(mesh, 54);
    vAnkleR = getBoneLoc(mesh, 58);
    float dis = caculDis(vNeck, vChest);
    vHead.X = vNeck.X + (0.1f * (vNeck.X - vChest.X) / dis);
    vHead.Y = vNeck.Y + (0.1f * (vNeck.Y - vChest.Y) / dis);
    vHead.Z = vNeck.Z + (0.1f * (vNeck.Z - vChest.Z) / dis);
    WorldToScreenPoint(vHead, skeleton->Head);
    WorldToScreenPoint(vNeck, skeleton->vNeck);
    WorldToScreenPoint(vChest, skeleton->Chest);
    WorldToScreenPoint(vPelvis, skeleton->Pelvis);
    WorldToScreenPoint(vShoulderL, skeleton->Left_Shoulder);
    WorldToScreenPoint(vShoulderR, skeleton->Right_Shoulder);
    WorldToScreenPoint(vElbowL, skeleton->Left_Elbow);
    WorldToScreenPoint(vElbowR, skeleton->Right_Elbow);
    WorldToScreenPoint(vWristL, skeleton->Left_Wrist);
    WorldToScreenPoint(vWristR, skeleton->Right_Wrist);
    WorldToScreenPoint(vLegL, skeleton->Left_Thigh);
    WorldToScreenPoint(vLegR, skeleton->Right_Thigh);
    WorldToScreenPoint(vKneeL, skeleton->Left_Knee);
    WorldToScreenPoint(vKneeR, skeleton->Right_Knee);
    WorldToScreenPoint(vAnkleL, skeleton->Left_Ankle);
    WorldToScreenPoint(vAnkleR, skeleton->Right_Ankle);
}


void Box3D2(FVector origin, FVector extends, struct Draw3DBox *draw3DBox) {
    origin -= extends / 2.f;

    // bottom plane
    FVector one = origin;
    FVector two = origin;
    two.x += extends.x;
    FVector three = origin;
    three.x += extends.x;
    three.y += extends.y;
    FVector four = origin;
    four.y += extends.y;

    FVector five = one;
    five.z += extends.z;
    FVector six = two;
    six.z += extends.z;
    FVector seven = three;
    seven.z += extends.z;
    FVector eight = four;
    eight.z += extends.z;

    FVector s1, s2, s3, s4, s5, s6, s7, s8;


    if (!worldToScreen(&one, &s1) || !worldToScreen(&two, &s2) || !worldToScreen(&three, &s3) ||
        !worldToScreen(&four, &s4))
        return;

    if (!worldToScreen(&five, &s5) || !worldToScreen(&six, &s6) || !worldToScreen(&seven, &s7) ||
        !worldToScreen(&eight, &s8))
        return;

    draw3DBox->v1.X = s1.x;
    draw3DBox->v1.Y = s1.y;
    draw3DBox->v2.X = s2.x;
    draw3DBox->v2.Y = s2.y;
    draw3DBox->v3.X = s3.x;
    draw3DBox->v3.Y = s3.y;
    draw3DBox->v4.X = s4.x;
    draw3DBox->v4.Y = s4.y;
    draw3DBox->v5.X = s5.x;
    draw3DBox->v5.Y = s5.y;
    draw3DBox->v6.X = s6.x;
    draw3DBox->v6.Y = s6.y;
    draw3DBox->v7.X = s7.x;
    draw3DBox->v7.Y = s7.y;
    draw3DBox->v8.X = s8.x;
    draw3DBox->v8.Y = s8.y;
}

/**
 *
 * @param mesh
 * @param bone
 * @param skeleton
 */
void DrawBone(uintptr_t mesh, uintptr_t bone, BoneData *skeleton) {

    Vector3A vHead, vChest, vNeck, vPelvis, vShoulderL, vShoulderR, vElbowL, vElbowR, vWristL, vWristR, vLegL, vLegR, vKneeL, vKneeR, vAnkleL, vAnkleR;
    getBoneLocation(mesh, vHead, bone + 6 * 48);
    getBoneLocation(mesh, vNeck, bone + 5 * 48);
    getBoneLocation(mesh, vChest, bone + 4 * 48);
    getBoneLocation(mesh, vPelvis, bone + 1 * 48);
    getBoneLocation(mesh, vShoulderL, bone + 11 * 48);
    getBoneLocation(mesh, vShoulderR, bone + 32 * 48);
    getBoneLocation(mesh, vElbowL, bone + 12 * 48);
    getBoneLocation(mesh, vElbowR, bone + 33 * 48);
    getBoneLocation(mesh, vWristL, bone + 63 * 48);
    getBoneLocation(mesh, vWristR, bone + 62 * 48);
    getBoneLocation(mesh, vLegL, bone + 52 * 48);
    getBoneLocation(mesh, vLegR, bone + 56 * 48);
    getBoneLocation(mesh, vKneeL, bone + 53 * 48);
    getBoneLocation(mesh, vKneeR, bone + 57 * 48);
    getBoneLocation(mesh, vAnkleL, bone + 54 * 48);
    getBoneLocation(mesh, vAnkleR, bone + 58 * 48);


    float dis = caculDis(vNeck, vChest);
    vHead.X = vNeck.X;//+ (0.1f * (vNeck.X - vChest.X) / dis);
    vHead.Y = vNeck.Y;//+ (0.1f * (vNeck.Y - vChest.Y) / dis);
    vHead.Z = vNeck.Z + 7;//+ (0.1f * (vNeck.Z - vChest.Z) / dis);
    WorldToScreenPoint(vHead, skeleton->Head);
    WorldToScreenPoint(vNeck, skeleton->vNeck);
    WorldToScreenPoint(vChest, skeleton->Chest);
    WorldToScreenPoint(vPelvis, skeleton->Pelvis);
    WorldToScreenPoint(vShoulderL, skeleton->Left_Shoulder);
    WorldToScreenPoint(vShoulderR, skeleton->Right_Shoulder);
    WorldToScreenPoint(vElbowL, skeleton->Left_Elbow);
    WorldToScreenPoint(vElbowR, skeleton->Right_Elbow);
    WorldToScreenPoint(vWristL, skeleton->Left_Wrist);
    WorldToScreenPoint(vWristR, skeleton->Right_Wrist);
    WorldToScreenPoint(vLegL, skeleton->Left_Thigh);
    WorldToScreenPoint(vLegR, skeleton->Right_Thigh);
    WorldToScreenPoint(vKneeL, skeleton->Left_Knee);
    WorldToScreenPoint(vKneeR, skeleton->Right_Knee);
    WorldToScreenPoint(vAnkleL, skeleton->Left_Ankle);
    WorldToScreenPoint(vAnkleR, skeleton->Right_Ankle);
}

/**
 * ????????????
 * @param mesh
 * @param bone
 * @return
 */
static Vector3A aimBoneLocation(uintptr_t mesh, uintptr_t bone) {
    BoneData boneData;
    getBoneLocation(mesh, boneData.Chest, bone + 4 * 48);
    if (isVisiblePoint(boneData.Chest)) {
        return boneData.Chest;
    }
    getBoneLocation(mesh, boneData.Pelvis, bone + 1 * 48);
    if (isVisiblePoint(boneData.Pelvis)) {
        return boneData.Pelvis;
    }
    getBoneLocation(mesh, boneData.Left_Shoulder, bone + 11 * 48);
    if (isVisiblePoint(boneData.Left_Shoulder)) {
        return boneData.Left_Shoulder;
    }
    getBoneLocation(mesh, boneData.Right_Shoulder, bone + 32 * 48);
    if (isVisiblePoint(boneData.Right_Shoulder)) {
        return boneData.Right_Shoulder;
    }
    getBoneLocation(mesh, boneData.Left_Elbow, bone + 12 * 48);
    if (isVisiblePoint(boneData.Left_Elbow)) {
        return boneData.Left_Elbow;
    }
    getBoneLocation(mesh, boneData.Right_Elbow, bone + 33 * 48);
    if (isVisiblePoint(boneData.Right_Elbow)) {
        return boneData.Right_Elbow;
    }
    getBoneLocation(mesh, boneData.Left_Wrist, bone + 63 * 48);
    if (isVisiblePoint(boneData.Left_Wrist)) {
        return boneData.Left_Wrist;
    }
    getBoneLocation(mesh, boneData.Right_Wrist, bone + 62 * 48);
    if (isVisiblePoint(boneData.Right_Wrist)) {
        return boneData.Right_Wrist;
    }
    getBoneLocation(mesh, boneData.Left_Thigh, bone + 52 * 48);
    if (isVisiblePoint(boneData.Left_Thigh)) {
        return boneData.Left_Thigh;
    }
    // return {0, 0, 0};
    getBoneLocation(mesh, boneData.Right_Thigh, bone + 56 * 48);
    if (isVisiblePoint(boneData.Right_Thigh)) {
        return boneData.Right_Thigh;
    }
    getBoneLocation(mesh, boneData.Left_Knee, bone + 53 * 48);
    if (isVisiblePoint(boneData.Left_Knee)) {
        return boneData.Left_Knee;
    }
    getBoneLocation(mesh, boneData.Right_Knee, bone + 57 * 48);
    if (isVisiblePoint(boneData.Right_Knee)) {
        return boneData.Right_Knee;
    }
    getBoneLocation(mesh, boneData.Left_Ankle, bone + 54 * 48);
    if (isVisiblePoint(boneData.Left_Ankle)) {
        return boneData.Left_Ankle;
    }
    getBoneLocation(mesh, boneData.Right_Ankle, bone + 58 * 48);
    if (isVisiblePoint(boneData.Right_Ankle)) {
        return boneData.Right_Ankle;
    }

    return {0, 0, 0};

}

//
// Created by admin on 2022/2/20.
//
#include "GameUtils.h"

using namespace std;

//void DrawLine(int x1, int y1, int x2, int y2, ImVec4 color, int size)
//??????????????????
static bool (*LineOfSightTo)(void *controller, void *actor, Vector3A ViewPoint,
                             bool bAlternateChecks) = nullptr;

static Vector3A (*GetBoneLocation)(void *mesh, int BoneName, char Space) = nullptr;// 0x67c1a18
static int (*GetBoneName)(void *mesh, int BoneIndex) = nullptr;// 0x67c1ae4
static void (*GetActorBounds)(void *actorBase, bool bOnlyCollidingComponents, const FVector &Origin,
                              const FVector &BoxExtent) = nullptr;// ;sub_5CE67A4
static void
(*ClientSetLocation)(void *actorBase, Vector3A NewLocation, Vector3A NewRotation) = nullptr;

static void (*SetControlRotation)(void *actorBase, const Vector3A &rotation) = nullptr;

static void (*AddControllerYawInput)(void *myBase, float val) = nullptr;

static void (*AddControllerPitchInput)(void *myBase, float val) = nullptr;

static void
(*SetUseViewTranslatedTransform)(void *myBase, bool bNewUseViewTranslatedTransform) = nullptr;


//??????????????????
static uintptr_t playerController = 0;
static uintptr_t uWorldAddress = 0;
static uintptr_t PersistentLevel = 0;
static uintptr_t NetDriver = 0;
static uintptr_t uMyObject = 0;
//????????????
static int SelfTeamID = 0;
static int aimCount = 0;

//??????????????????
static int actorsCount = 0;
static Update update[MaxActorCount + 5];
static Update update_data[MaxActorCount + 10];


static int updateActorsCount = 0;
static bool isRead = false;
static bool isSend = false;
static int BoneIndexs[65] = {0};
//GworldAddr
uintptr_t libUE4 = 0;
uintptr_t GnameAddress = 0;
int PlayerCount = 0;
int VehicleCount = 0;
int ItemsCount = 0;
int AirBoxCount = 0;
int GrenadeCount = 0;
//????????????
static Vector3A SelfInfo;


//????????????
static bool isPeople = true;
static bool isItems = true;
static bool isVehicle = true;
static bool isMissiles = true;

//??????
static int bIsGunADS = 0;
static int myIsGunADS = 0;
int myBIsPressingFireBtn = 0;

int my_aimedMode = 0;//????????????
int bIsPressingFireBtn = 0;
static uintptr_t aimData = 0;
static float ShakeValue = 0;//???????????????
static float touchShakeValue = 0;
static float Orientation = 1.0f;//??????????????????
static bool isMyProjSomoke = false;// ????????????
//????????????
static int aiNum = 0, playerNum = 0;
extern int screenWidth, screenHeight;
extern float gameFov;
extern int glWidth, glHeight;
bool openAccumulation;

/**
 * ??????????????????
 * @param Mesh
 * @param BoneIndex
 * @return
 */
Vector3A getBoneLoc(uintptr_t Mesh, int BoneIndex) {
    if (isMincore(Mesh, Offset::PointerSize) != 1) { return {0, 0, 0}; }
    BoneIndex += 1;
    if (GetBoneLocation != nullptr && GetBoneName != nullptr) {
        if (Mesh > 0x1000000 && Mesh % 4 == 0 && Mesh < 0x10000000000) {
            int Name = BoneIndexs[BoneIndex];
            if (Name <= 0) {
                Name = GetBoneName(reinterpret_cast<void *>(Mesh), BoneIndex);
                BoneIndexs[BoneIndex] = Name;
            }
            return GetBoneLocation(reinterpret_cast<void *>(Mesh), Name, 0);
        }
    }
    return {0, 0, 0};
}

/**
 *
 *????????????
 * @param point
 * @return
 */
bool isSafeAddr(Vector3A point) {
    if (isnan(point.X) || isnan(point.Y) || isnan(point.Z) || point.X < 1 || point.Y < 1 ||
        point.Z < 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * ????????????
 */
bool isVisiblePoint(Vector3A point) {
    if (isSafeAddr(point)) {
        return false;
    }
    if (playerController < 0x100000 || playerCameraManagerBase < 0x100000) {
        return false;
    }

    string playerControllerClassName = getName(playerController, GnameAddress);
    string cameraManagerClassName = getName(playerCameraManagerBase, GnameAddress);

    if (LineOfSightTo != nullptr && isContain(playerControllerClassName, "PlayerController") &&
        isContain(cameraManagerClassName, "CameraManager")) {
        return LineOfSightTo(reinterpret_cast<void *>(playerController),
                             reinterpret_cast<void *>(playerCameraManagerBase), point, false);
    }
    return false;
}


/**
 * ??????????????????
 * @param mesh
 * @return
 */
static Vector3A aimBoneLocation(uintptr_t mesh) {
    BoneData boneData;
    boneData.Chest = getBoneLoc(mesh, 4);
    if (isVisiblePoint(boneData.Chest)) {
        return boneData.Chest;
    }
    boneData.Pelvis = getBoneLoc(mesh, 1);
    if (isVisiblePoint(boneData.Pelvis)) {
        return boneData.Pelvis;
    }
    boneData.Left_Shoulder = getBoneLoc(mesh, 11);
    if (isVisiblePoint(boneData.Left_Shoulder)) {
        return boneData.Left_Shoulder;
    }
    boneData.Right_Shoulder = getBoneLoc(mesh, 32);
    if (isVisiblePoint(boneData.Right_Shoulder)) {
        return boneData.Right_Shoulder;
    }
    boneData.Left_Elbow = getBoneLoc(mesh, 12);
    if (isVisiblePoint(boneData.Left_Elbow)) {
        return boneData.Left_Elbow;
    }
    boneData.Right_Elbow = getBoneLoc(mesh, 33);

    if (isVisiblePoint(boneData.Right_Elbow)) {
        return boneData.Right_Elbow;
    }
    boneData.Left_Wrist = getBoneLoc(mesh, 63);

    if (isVisiblePoint(boneData.Left_Wrist)) {
        return boneData.Left_Wrist;
    }
    boneData.Right_Wrist = getBoneLoc(mesh, 62);
    if (isVisiblePoint(boneData.Right_Wrist)) {
        return boneData.Right_Wrist;
    }
    boneData.Left_Thigh = getBoneLoc(mesh, 52);
    if (isVisiblePoint(boneData.Left_Thigh)) {
        return boneData.Left_Thigh;
    }
    // return {0, 0, 0};
    boneData.Right_Thigh = getBoneLoc(mesh, 56);
    if (isVisiblePoint(boneData.Right_Thigh)) {
        return boneData.Right_Thigh;
    }
    boneData.Left_Knee = getBoneLoc(mesh, 53);

    if (isVisiblePoint(boneData.Left_Knee)) {
        return boneData.Left_Knee;
    }
    boneData.Right_Knee = getBoneLoc(mesh, 57);
    if (isVisiblePoint(boneData.Right_Knee)) {
        return boneData.Right_Knee;
    }
    boneData.Left_Ankle = getBoneLoc(mesh, 54);

    if (isVisiblePoint(boneData.Left_Ankle)) {
        return boneData.Left_Ankle;
    }
    boneData.Right_Ankle = getBoneLoc(mesh, 58);
    if (isVisiblePoint(boneData.Right_Ankle)) {
        return boneData.Right_Ankle;
    }

    return {0, 0, 0};

}

bool isSafeAddr(float x, float y) {
    if (x != NAN && y != NAN && x > -1 && y > -1) {
        return true;
    } else {
        return false;
    }
}

/**
 * ???????????????????????????
 * @return
 */
bool isSafeAddr2(uintptr_t address) {
    //????????????
    uintptr_t ActorAddress = getA(PersistentLevel + 0x10 + Offset::ULevelToAActors);
    //??????
    int ResNum = getI(PersistentLevel + 0x10 + Offset::ULevelToAActorsCount);
    if (ResNum < 0 || ResNum > 29999) { return false; }
    for (int i = 0; i < ResNum; i++) {
        //????????????
        uintptr_t ObjectPointer = getA(ActorAddress + Offset::PointerSize * i);
        if (ObjectPointer == address) {
            return true;
        }
    }
    return false;
}

/**
 * ??????????????????
 * @param pitch
 * @param yaw
 */
static void AddControllerInput(const float *pitch, const float *yaw) {
    string uMyObjectClassName = getName(uMyObject, GnameAddress);
    if (AddControllerPitchInput != nullptr && AddControllerYawInput != nullptr &&
        isContain(uMyObjectClassName, "Pawn") && isSafeAddr(*pitch, *yaw)) {
        AddControllerPitchInput(reinterpret_cast<void *>(uMyObject), *pitch);
        AddControllerYawInput(reinterpret_cast<void *>(uMyObject), *yaw);

    }
}

/**
 * ????????????????????????
 * @param rotation
 */
static void my_SetControlRotation(Vector3A &rotation) {
    string playerControllerClassName = getName(playerController, GnameAddress);
    string cameraManagerClassName = getName(playerCameraManagerBase, GnameAddress);
    if (SetControlRotation != nullptr && isContain(playerControllerClassName, "PlayerController")) {
        SetControlRotation(reinterpret_cast<void *>(playerController), rotation);

    }
}


/**
 * ??????
 * @param ObjectPointer
 * @param ObjInfo
 */

static void setAimLocation(uintptr_t ObjectPointer, Vector3A ObjInfo) {

    float aimPress2 = aimPress / 100; //??????
    float aimAnticipation2 = aimAnticipation / 100; //??????

    //????????????
    Vector3A myLocation;
    memoryRead(playerCameraManagerBase + Offset::dw_CameraCacheEntry +
               Offset::dw_MinimalViewInfo + Offset::dw_Location, &myLocation,
               sizeof(myLocation));

    Vector4Screen tmpLocation;
    if (!worldToScreen(&ObjInfo, &tmpLocation)) {
        return;
    }
    //????????????
    Vector3A moveLocation;
    memoryRead(aimData + Offset::vectorOffset, &moveLocation,
               sizeof(moveLocation));
    //my???
    uintptr_t myGunBase = getA(
            getA(uMyObject + Offset::weaponOffset) + Offset::gunOffset);
    //??????
    double distance = sqrtf(powf(ObjInfo.X - myLocation.X, 2.0f) +
                            powf(ObjInfo.Y - myLocation.Y, 2.0f) +
                            powf(ObjInfo.Z - myLocation.Z, 2.0f));
    //????????????
    double aimoSpeed = getF(myGunBase + Offset::bulletVelocityOffset) * 0.01f;
    //????????????
    double shangTai =
            getF(getA(getA(uMyObject + Offset::weaponOffset) +
                      Offset::gunOffset) +
                 Offset::resistanceOffset) + aimPress2;

    int myState = getI(uMyObject + Offset::StateOffset);
    if (myState == 320) {
        //????????????
        shangTai /= 4;
    } else if (myState == 280 || myState == 4368 || myState == 4369) {
        //????????????
        shangTai /= 3;
    } else if (myState == 1312 || myState == 5392 || myState == 5393) {
        //????????????
        shangTai /= 1.15;
    } else if (myState == 1344) {
        //????????????
        shangTai /= 2.5;
    }

    //????????????
    int fireTypeInt = getI(
            getA(uMyObject + Offset::weaponOffset) + Offset::shootModeOffset);
    char fireType = (char) ((fireTypeInt >> 8) & 0xFF);
    if (fireType == 1) {
        shangTai = aimPress2 / 2.f;
    }

    int State = getI(ObjectPointer + Offset::StateOffset);
    if (State >= 8000000 && State <= 9000000) {
        shangTai = aimPress2 + 2;
    }

    //????????????
    double flyTime = (distance / 100.0f) / aimoSpeed; //???
    //?????????
    double dropM = 0.0f;
    //?????????????????????
    double useS = flyTime + aimAnticipation2;
    Vector3A yupanPoint;
    yupanPoint.X = ObjInfo.X + moveLocation.X * useS;
    yupanPoint.Y = ObjInfo.Y + moveLocation.Y * useS;
    yupanPoint.Z = ObjInfo.Z + moveLocation.Z * useS + dropM;

    double yuPanDistance =
            sqrtf(powf(yupanPoint.X - myLocation.X, 2.0f) +
                  powf(yupanPoint.Y - myLocation.Y, 2.0f) +
                  powf(yupanPoint.Z - myLocation.Z, 2.0f)) * 0.01f;
    double realFlyTime = yuPanDistance / aimoSpeed;
    double realUseS = realFlyTime + aimAnticipation2;
    yupanPoint.X = ObjInfo.X + moveLocation.X * realUseS;
    yupanPoint.Y = ObjInfo.Y + moveLocation.Y * realUseS;
    yupanPoint.Z = ObjInfo.Z + moveLocation.Z * realUseS + dropM;

    double zDistance = (myLocation.Z - yupanPoint.Z) * 0.01f;
    if (zDistance > 0.0f) {
        shangTai = shangTai + zDistance * 0.00147f;
    }
    double vax = 180.0f / PI;
    double vay = 180.0f / PI;
    double cx = yupanPoint.X - myLocation.X;
    double cy = yupanPoint.Y - myLocation.Y;
    double cz = yupanPoint.Z - myLocation.Z;
    double pfg = sqrtf((cx * cx) + (cy * cy));

    float max_shake2 = max_shake;//= 0.5f + (float) (random() % 100) / 100.0f;//0.8- 0.99

    ShakeValue = ShakeValue + 0.05f * Orientation;
    if (fabs(ShakeValue) >= max_shake2) {
        Orientation = -Orientation;
    }
    double XGX, XGY;
    if (max_shake2 > 0) {
        XGX = atan2f(cy, cx) * vax + (fireType == 1 ? 0.0f : ShakeValue);
        XGY = atan2f(cz, pfg) * vay + (fireType == 1 ? 0.0f : ShakeValue);
    } else {
        XGX = atan2f(cy, cx) * vax;
        XGY = atan2f(cz, pfg) * vay;
    }
    if (XGX >= -180.0f && XGX < 0.0f) {
        XGX = XGX + 360.0f;
    }
    if (XGY < 0.0f && XGY >= -180.0f) {
        XGY = XGY + 360.0f;
    }
    XGY = XGY - shangTai;
    bool isOKYValue = XGY != NAN && XGX != NAN;

    if (aimbot && isOKYValue && (bIsPressingFireBtn || bIsGunADS)) {
        if (XGY >= 360.0f) {
            XGY = XGY - 360.0f;
        }
        if (XGY < 0.0f) {
            XGY = XGY + 360.0f;
        }

        float currentRotationY = getF(
                playerController + Offset::controlRotationOffset + 0);//P
        float currentRotationX = getF(
                playerController + Offset::controlRotationOffset + 4);//Y

        float needAddY = XGY - currentRotationY;//P
        float needAddX = XGX - currentRotationX;//Y

        //358 = -2
        //181 = -179
        if (needAddX > 180.0f) {
            needAddX = needAddX - 360.0f;
        }
        //-358 = 2
        //-181 = 179
        if (needAddX < -180.0f) {
            needAddX = 360.0f + needAddX;
        }

        //358 = -2
        //181 = -179
        if (needAddY > 180.0f) {
            needAddY = needAddY - 360.0f;
        }
        //-358 = 2
        //-181 = 179
        if (needAddY < -180.0f) {
            needAddY = 360.0f + needAddY;
        }

        needAddX = needAddX / aimSpeed;
        needAddY = needAddY / aimSpeed * (aimSpeed == 1.0f ? 1.0f : 2.0f);

        XGX = currentRotationX + needAddX;
        XGY = currentRotationY + needAddY;
        //361
        if (XGX >= 360.0f) {
            XGX = XGX - 360.0f;
        }
        if (XGX < 0.0f) {
            XGX = XGX + 360.0f;
        }
        if (XGY >= 360.0f) {
            XGY = XGY - 360.0f;
        }
        if (XGY < 0.0f) {
            XGY = XGY + 360.0f;
        }
        //if (aimType == 0)
        {
            AddControllerInput(&needAddY, &needAddX);
        }
        //  if (aimType == 1) {
        //     Vector3A rotation;
        //     rotation.X = XGY;
        //     rotation.Y = XGX;
        //LOGD("x:%f - y:%f", XGY, XGX);
        //    my_SetControlRotation(rotation);
        // }
    }

}

/**
 * ??????????????????
 * @return
 */
[[noreturn]] void *updateDataList(void *) {
    prctl(PR_SET_NAME, "updateDataList");
    memset(BoneIndexs, 0, 65 * sizeof(int));
    while (true) {
        sleep(3);

        //??????ue4????????????
        if (libUE4 == 0) {
            LOGD("???????????????");
#ifdef __LP64__
            Offset::china64Offset();
#else
            Offset::chinaOffset32();
#endif
            sleep(5);
            LOGD("????????????????????????");
            libUE4 = getModule("libUE4.so", true);
            *(uintptr_t *) &GetActorBounds = (libUE4 + Offset::GetGetActorBoundsOffset);
            *(uintptr_t *) &GetBoneLocation = (libUE4 + Offset::GetBoneLocationOffset);
            *(uintptr_t *) &GetBoneName = (libUE4 + Offset::GetBoneNameOffset);
            *(uintptr_t *) &AddControllerYawInput = libUE4 + Offset::yaw_off;
            *(uintptr_t *) &AddControllerPitchInput = libUE4 + Offset::pitch_off;
            *(uintptr_t *) &LineOfSightTo = libUE4 + Offset::line_off;
            LOGD("libUE4:%lx", libUE4);
            continue;
        }
/*
        if (my_syscall == nullptr) {
#if defined(__LP64__)
            *(uintptr_t *) &my_syscall = FindPattern("libtersafe.so","E8 03 00 AA E0 03 01 AA E1 03 02 AA E2 03 03 AA E3 03 04 AA E4 03 05 AA E5 03 06 AA 01 00 00 D4");
#else
            *(uintptr_t *) &my_syscall =  FindPattern("libtersafe.so", "0D C0 A0 E1 F0 00 2D E9 00 70 A0 E1 01 00 A0 E1 02 10 A0 E1 03 20 A0 E1 78 00 9C E8 00 00 00 EF");
#endif
            syscall_load = true;
        }
*/
        //GName
        if (GnameAddress == 0) {
            GnameAddress = getA(libUE4 + Offset::GNames);
            continue;
        }
        //??????
        uWorldAddress = getA(libUE4 + Offset::GWorld);
        //??????????????????
        PersistentLevel = getA(uWorldAddress + Offset::persistentLevelOffset);
        // ?????????????????????
        NetDriver = getA(uWorldAddress + Offset::netDriverOffset);

        //??????????????????????????????????????????
        if (NetDriver == 0) {
            //??????????????????????????????
            //clearMap();
            isRead = false;
            LOGD("?????????:ue4:%lx - world:%lx", libUE4, uWorldAddress);
            continue;
        }

        isRead = true;
        //???????????????
        playerController = getA(
                getA(NetDriver + Offset::serverConnectionOffset) + Offset::playerControllerOffset);
        //????????????
        uMyObject = getA(playerController + Offset::uMyObjectOffset);
        //isPlayerController
        string playerControllerClassName = getName(playerController, GnameAddress);
        //??????????????????
        /*  if (isContain(playerControllerClassName, "PlayerController")) {
              uintptr_t vtable = getA(playerController + 0);
              if (vtable < 0x100000) { continue; }
              *(uintptr_t *) &LineOfSightTo = getA(
                      vtable + Offset::LineOfSightToOffset);//libUE4 + 0x5F79DE0;
              if (aimType == 1) {
                  *(uintptr_t *) &ClientSetLocation = getA(vtable + Offset::ClientSetLocationOffset);
                  *(uintptr_t *) &SetControlRotation = getA(vtable + Offset::SetControlRotationOffset);
              }
          }*/
        /*
        if (aimType == 0) {
            string uMyObjectClassName = getName(uMyObject);
            if (isContain(uMyObjectClassName, "Pawn")) {
                uintptr_t vtable = getA(uMyObject + 0x0);
                if (vtable < 0x100000) { continue; }
                *(uintptr_t *) &AddControllerPitchInput = getA(vtable + Offset::AddPitchInputOffset);
                *(uintptr_t *) &AddControllerYawInput = getA(vtable + Offset::AddYawInputOfffset);

                //LOGD("pitch_off:%lx",*(uintptr_t *) &AddControllerPitchInput - libUE4);
                //LOGD("Yaw_off:%lx",*(uintptr_t *) &AddControllerYawInput - libUE4);
                //LOGD("line_off:%lx",*(uintptr_t *) &LineOfSightTo - libUE4);

            }
        }*/

        //????????????
        playerCameraManagerBase = getA(playerController + Offset::playerCameraManagerOffset);

        //????????????
        uintptr_t ActorAddress = getA(PersistentLevel + Offset::ULevelToAActors);
        //????????????
        int ResNum = getI(PersistentLevel + Offset::ULevelToAActorsCount);

        actorsCount = 0;
        memset(update, 0, (MaxActorCount + 5) * sizeof(Update));
        //????????????????????????
        for (int i = 0; i < ResNum; i++) {
            if (actorsCount >= MaxActorCount) {
                continue;
            }
            // ????????????
            uintptr_t ObjectPointer = getA(ActorAddress + Offset::PointerSize * i);
#ifdef __LP64__
            if (ObjectPointer <= 0x10000000 || ObjectPointer % 4 != 0 ||
                ObjectPointer >= 0x10000000000) {
                continue;
            }
#else
            if (ObjectPointer <= 0x100000 || ObjectPointer % 4 != 0 ||
                ObjectPointer >= 0x10000000) {
                continue;
            }
#endif
            //  if (isMincoreSafeAddr(ObjectPointer) != 1) { continue; }

            if (!isSafeAddr2(ObjectPointer)) { continue; }
            //?????????????????? 4096 * 3???
            if (isMincore(ObjectPointer, 0x3000) != 1) {
                continue;
            }
            // ????????????
            uintptr_t RootCompont = getA(ObjectPointer + Offset::CoordinatePointerOffset);
#ifdef __LP64__
            if (RootCompont <= 0x10000000 || RootCompont % 4 != 0 || RootCompont >= 0x10000000000) {
                continue;
            }
#else
            if (RootCompont <= 0x100000 || RootCompont % 4 != 0 || RootCompont >= 0x10000000) {
                continue;
            }
#endif
            update[actorsCount].RootCompont = RootCompont;

            //??????????????????
            update[actorsCount].ItemName = getName(ObjectPointer, GnameAddress);
            //????????????
            uintptr_t clazz = getA(ObjectPointer + Offset::UObjectToClassPrivate);
            string ClassPath = update[actorsCount].ItemName;

            uintptr_t superclass = getA(clazz + Offset::UStructToSuperStruct);
            while (superclass) {
                ClassPath += ".";
                ClassPath += getName(superclass, GnameAddress);
                superclass = getA(superclass + Offset::UStructToSuperStruct);
            }

            if (isContain(update[actorsCount].ItemName, "Recycled")) { continue; }

            update[actorsCount].ClassPath = ClassPath;

            // ??????
            if (isContain(ClassPath, "STExtraPlayerCharacter")) {
                if (!isPeople)
                    continue;
                //no head
                if (getA(ObjectPointer + Offset::CustomSpringArmComponentOffset) == 0) { continue; }
                //isPlayer

                // ????????????
                int bDead = getI(ObjectPointer + Offset::bDeadOffset);
                if (!bDead) { continue; }
                if (getF(ObjectPointer + Offset::MaxDistFromMainCharOffset) !=
                    9000000.0f) { continue; }

                uintptr_t mesh_ = getA(ObjectPointer + Offset::BoneAddrOffset);
                if (!isContain(getName(mesh_, GnameAddress), "CharacterMesh0")) {
                    continue;
                }
                //????????????
                update[actorsCount].isAI = getI(ObjectPointer + Offset::isBotOffset) == 1;
                //??????????????????
                update[actorsCount].isRat = isContain(ClassPath, "Impostors");
                if (!update[actorsCount].isAI) {
                    //????????????
                    uintptr_t NamePointer = getA(ObjectPointer + Offset::PlayerNameOffset);
                    getPlayerName(update[actorsCount].PlayerName, NamePointer);
                    if (strlen(update[actorsCount].PlayerName) <= 0) {
                        continue;
                    }
                }
                //????????????
                update[actorsCount].TeamID = getI(ObjectPointer + Offset::TeamIDOffset);

                update[actorsCount].plater_mesh_ = mesh_;
                //bone
                update[actorsCount].plater_bone =
                        getA(mesh_ + Offset::BoneOffset) + Offset::perLODBiasTypeInfoOffset;
                //mesh
                update[actorsCount].plater_mesh = mesh_ + Offset::BoneMatrixOffset;

                //update[actorsCount].player_controllers = getA(ObjectPointer + Offset::PlayerControllerOffset);

                update[actorsCount].actorBases = ObjectPointer;

                actorsCount++;
            }
                // ??????
            else if (isContain(ClassPath, "STExtraVehicleBase") ||
                     isContain(ClassPath, "FlightVehicle")) {
                if (!isVehicle)
                    continue;
                update[actorsCount].actorBases = ObjectPointer;
                update[actorsCount].vehicleCommonComponent = getA(
                        ObjectPointer + Offset::vehicleCommonComponentOffset);
                actorsCount++;
            }
                // ???????????????->>
            else if (isContain(ClassPath, "PickUpListWrapperActor") ||
                     isContain(ClassPath, "AirDropBoxActor")) {
                if (!showChest && !showDrop)continue;
                update[actorsCount].actorBases = ObjectPointer;
                actorsCount++;
            }
                // ??????????????????
            else if (isContain(ClassPath, "PickUpWrapperActor")) {
                if (!isItems)
                    continue;
                update[actorsCount].actorBases = ObjectPointer;
                actorsCount++;
            }
                // ???????????????
            else if (isContain(ClassPath, "EliteProjectile")) {
                if (!isMissiles)
                    continue;
                update[actorsCount].actorBases = ObjectPointer;
                actorsCount++;
            }
        }

        isRead = false;
    }
}


/**
 * ??????????????????
 */
void createDataList() {
    if (libUE4 == 0) { return; }
    // py = screenHeight / 2 , px = screenWidth / 2 , screenSizeX =screenWidth , screenSizeY = screenHeight ;
    //???????????????
    px = glWidth / 2, py = glHeight / 2, screenSizeX = glWidth, screenSizeY = glHeight;
    //??????????????????
    isItems = showItem;
    isPeople = true;
    isMissiles = showGrenade;
    isVehicle = showVehicle;
    my_aimedMode = aimedMode;
    PlayerCount = 0;
    VehicleCount = 0;
    ItemsCount = 0;
    AirBoxCount = 0;
    GrenadeCount = 0;
    // ?????????????????????
    NetDriver = getA(uWorldAddress + Offset::netDriverOffset);


    if (NetDriver == 0) {
        DrawStrokeText((int) screenWidth / 3, 120, Color.White, "??????????????????");
        //??????????????????????????????
        // clearMap();
        return;
    }

    DrawNum(aiNum, playerNum);
    initDraw();

    //????????????
    aiNum = 0, playerNum = 0;
    // if (isRead) {return;}
    //????????????
    int tempTid = getI(uMyObject + Offset::TeamIDOffset);
    if (tempTid <= 100 && tempTid > 0) { SelfTeamID = tempTid; }

    //??????????????????????????????????????? ??????????????? ?????? ??????
    if (my_aimedMode == 3 && aimbot) {
        int fireTypeInt = getI(getA(uMyObject + Offset::weaponOffset) + Offset::shootModeOffset);
        char fireType = (char) ((fireTypeInt >> 8) & 0xFF);
        //????????????????????????
        if (fireType == 1) { my_aimedMode = 0; } else { my_aimedMode = 1; }
    }

    //????????????????????????
    memoryRead(uMyObject + Offset::adsOffset, &myIsGunADS, 1);
    if(isSelectedAim){
        //??????fov
        gameFov = CameraManager::GetFov() / 100;
        //????????????????????????
        myBIsPressingFireBtn = getI(playerController + Offset::fireButtonOffset);
    }

    //???????????????????????????????????????
    if (!myIsGunADS && getA(getA(uMyObject + Offset::weaponOffset) + Offset::gunOffset) != 0 &&
        showCrosshair)
        openAccumulation = true;
    else { openAccumulation = false; }

    float nearest = 9999.0f;
    bIsGunADS = 0;
    bIsPressingFireBtn = 0;
    uintptr_t tempAddr = 0;
    //?????????????????????
    isSend = true;
    //??????????????????
    if (actorsCount > 0 && !isRead) {
        updateActorsCount = actorsCount;
        memset(update_data, 0, updateActorsCount * sizeof(Update));
        my_memmove(update_data, update, updateActorsCount * sizeof(Update));
    }
    for (int i = 0; i < updateActorsCount; i++) {
        // ????????????
        uintptr_t ObjectPointer = update_data[i].actorBases;
        if (ObjectPointer == 0) { continue; }
        if (isMincore(ObjectPointer, 0x3000) != 1) { continue; }
        // ????????????(?????????)
        uintptr_t RootCompont = update_data[i].RootCompont;
        if (RootCompont == 0) { continue; }
        //???????????????????????????????????????????????????
        if (isMincore(RootCompont, Offset::PointerSize) != 1) {
            continue;
        }
        if (isContain(update_data[i].ClassPath, "STExtraPlayerCharacter")) {
            if (!isPeople)
                continue;
            if (update_data[i].plater_mesh_ == 0) { continue; }
            if (!isContain(getName(update_data[i].plater_mesh_, GnameAddress), "CharacterMesh0") ||
                isMincore(update_data[i].plater_mesh_, Offset::PointerSize) != 1) {
                continue;
            }
            //??????
            memoryRead(playerCameraManagerBase + Offset::dw_CameraCacheEntry +
                       Offset::dw_MinimalViewInfo + Offset::dw_Location, &SelfInfo,
                       sizeof(SelfInfo));
            // ????????????
            int teamID = update_data[i].TeamID;
            // ????????????
            if ((teamID == SelfTeamID || teamID <= 0) &&
                !isContain(update_data[i].ClassPath, "WerewolfBase")) {
                continue;
            }
            // ????????????????????????????????????
            if (PlayerCount >= MaxPlayerCount) { continue; }

            PlayerData data;
            //??????
            data.TeamID = teamID;
            // ????????????????????????
            /* ???????????? */
            Vector3A ObjInfo;
            //uintptr_t bone = update_data[i].plater_bone;
            //uintptr_t mesh = update_data[i].plater_mesh;
            uintptr_t mesh_ = update_data[i].plater_mesh_;
            ObjInfo = getBoneLoc(mesh_, 5);
            //getBoneLocation(mesh, ObjInfo, bone + 5 * 48);
            ObjInfo.Z += 7;
            if (isnan(ObjInfo.X) || isinf(ObjInfo.X) || isnan(ObjInfo.Y) || isinf(ObjInfo.Y) ||
                isnan(ObjInfo.Z) || isinf(ObjInfo.Z)) {
                continue;
            }

            //????????????
            bool isShow = worldToScreen(&ObjInfo, &data.Location);

            //????????????????????????
            data.HeadSize = data.Location.h / 6.4f;//6.4f;

            // ????????????
            data.Angle = getF(ObjectPointer + Offset::RotationAngleOffset);

            data.Angle = data.Angle + 90.0f;

            if (data.Angle < 0) { data.Angle = 360.0f + data.Angle; }

            // ????????????_??????
            data.RotationAngle = rotateCoord(
                    getF(uMyObject + Offset::RotationAngleOffset) - 90.0f,
                    (SelfInfo.X - ObjInfo.X) / 225, (SelfInfo.Y - ObjInfo.Y) / 225);

            // ????????????
            data.RadarLocation = Vector2A(ObjInfo.X - SelfInfo.X, ObjInfo.Y - SelfInfo.Y);

            // ????????????
            float HelathInfo[3] = {0};
            memoryRead(ObjectPointer + Offset::HealthOffset, HelathInfo, sizeof(HelathInfo));
            data.Health = HelathInfo[0] / HelathInfo[2] * 100;
            // ????????????
            data.isAI = update_data[i].isAI;
            data.isAI ? aiNum++ : playerNum++;
            //??????????????????
            data.isRat = update_data[i].isRat;
            // ????????????
            data.State = getI(ObjectPointer + Offset::StateOffset);
            //????????????
            data.Distance = (float) data.Location.d;
            if (data.Distance > 500.0) { continue; }
            // ????????????
            if (data.State == 1048576 || data.State == 0 || data.State == 1048577 ||
                data.State == 1048592 || data.State == 71894088) { continue; }

            if (!isShow) { data.Location.d = 1; }
            //????????????
            drawRadar(data.Distance, data.RotationAngle.X, data.RotationAngle.Y, data.isAI,
                      getisDie(data.State, data.Health), radarOffsetX, radarOffsetY, data.RadarLocation.X,  data.RadarLocation.Y,  data.Angle );


            //?????????????????? ?????? ????????????
            if (isShow && data.Location.x > 0 && data.Location.x < screenWidth &&
                data.Location.y > 0 &&
                data.Location.y < screenHeight && data.State != 1048576 && data.Distance >= 4) {

                if (showBox) {
                    //??????3D?????????????????????
                    int PoseType;
                    memoryRead(ObjectPointer + Offset::PoseStateOffset, &PoseType, 1);

                    FVector origin, extends;

                    Vector3A center, Right_Ankle, Left_Wrist;
                    memoryRead(RootCompont + Offset::CoordinateOffset, &center, sizeof(ObjInfo));

                    //??????
                    Right_Ankle = getBoneLoc(mesh_, 58);
                    //getBoneLocation(mesh, Right_Ankle, bone + 58 * 48);
                    //??????
                    Left_Wrist = getBoneLoc(mesh_, 63);
                    //getBoneLocation(mesh, Left_Wrist, bone + 63 * 48);

                    if (Right_Ankle.X > Left_Wrist.X) {
                        extends.x = float((float) (Right_Ankle.X - Left_Wrist.X) * 2);
                    } else {
                        extends.x = float((float) (Left_Wrist.X - Right_Ankle.X) * 2);
                    }
                    if (Right_Ankle.Y > Left_Wrist.Y) {
                        extends.y = float((float) (Right_Ankle.Y - Left_Wrist.Y) * 2);
                    } else {
                        extends.y = float((float) (Left_Wrist.Y - Right_Ankle.Y) * 2);
                    }
                    extends.z = float((float) (Right_Ankle.Z - ObjInfo.Z));
                    extends.z -= 8;
                    if (extends.x < 100) {
                        extends.x = 110;
                    }
                    if (extends.x > 300) {
                        extends.x = 185;
                    }
                    if (extends.y < 100) {
                        extends.y = 110;
                    }
                    if (extends.y > 300) {
                        extends.y = 185;
                    }
                    if (extends.z > 0) {
                        extends.z = 70;
                    }
                    if (extends.z > -100) {
                        extends.z = -120;
                    }
                    Box3D2(FVector(center.X, center.Y, center.Z), extends, &data.draw3DBox);
                }
                //?????????????????????????????????
                if (isObjAimed) {
                    Vector2A AimFov;
                    AimFov.Y = getF(ObjectPointer + Offset::RotationAngleOffset);
                    Vector2A angle = CalcAngle(ObjInfo, SelfInfo);
                    FRotator AmibitLoc = FRotator(0, AimFov.Y, 0);
                    AmibitLoc.Clamp();
                    data.objAim.X = abs(angle.X - AmibitLoc.Yaw);
                    data.objAim.Y = abs(angle.Y - AmibitLoc.Pitch);
                }
                //????????????????????????
                //????????????
                data.Weapon = getPlayerWeapon(ObjectPointer);
                //????????????
                if (showBone) {
                    //DrawBone(mesh, bone, &(data.mBoneData));
                    DrawBone(mesh_, &(data.mBoneData));
                }
                //????????????
                if (isVisibility) {
                    data.isVisibility = isVisiblePoint(ObjInfo);
                }
                //??????????????????????????????????????????????????? ??????????????????????????????
                if (isRunningLoc) {
                    //??????????????????
                    Vector3A running, runningLoc;
                    runningLoc.X = ObjInfo.X;
                    runningLoc.Y = ObjInfo.Y;
                    runningLoc.Z = ObjInfo.Z;
                    //?????????????????? ??????
                    memoryRead(ObjectPointer + Offset::vectorOffset, &running, sizeof(running));
                    //??????????????????
                    float bulletVelocity = getF(
                            getA(getA(uMyObject + Offset::weaponOffset) + Offset::gunOffset) +
                            Offset::bulletVelocityOffset);
                    float time = data.Distance / (bulletVelocity * 0.01f);
                    //??????????????????+????????????
                    runningLoc.X += running.X * time;
                    runningLoc.Y += running.Y * time;
                    runningLoc.Z += running.Z * time + 0.5f * 588.6f * time * time;
                    worldToScreen(&runningLoc, &data.runningLoc);
                }

                Rect box;
                box.x = data.Location.x - data.Location.h / 4;
                box.y = data.Location.y;//obj.draw3DBox.v8.Y;//
                box.h = data.Location.h;
                box.w = data.Location.h / 2;
                //????????????
                DrawInfo(box, data.Health, data.TeamID, update_data[i].PlayerName, data.Distance,
                         data.isRat,
                         data.isAI,
                         data.Weapon.id, data.isVisibility, getisDie(data.State, data.Health),
                         data);
            } else {
                //????????????
                drawWarning(data.Location.x, data.Location.y, (int) data.Distance);
            }

            PlayerCount++;
            //??????180??FOV ??????
            //?????????????????? ????????????
            if (aimbot) {
                //???????????? ?????? = 1 ?????? = 1||257 || 256 ?????? 0
                if (my_aimedMode == 2 || my_aimedMode == 1) {
                    bIsPressingFireBtn = getI(playerController + Offset::fireButtonOffset);
                }
                if (my_aimedMode == 2 || my_aimedMode == 0) {
                    memoryRead(uMyObject + Offset::adsOffset, &bIsGunADS, 1);
                }
            }
            if (isShow && (dieNoAim || data.Health > 0) && (aimbot) && aimData == 0) {
                float centerDist = get2dDistance(px, py, data.Location.x, data.Location.y);
                if (centerDist < (float) aimRange) {
                    //??????????????????????????????3d??????
                    if (aimChoose != 0 && aimChoose != 1)
                        centerDist = data.Distance;
                    else if (aimChoose == 1) {
                        if (data.Distance < 50) {
                            centerDist = data.Distance;
                        } else {
                            centerDist = get2dDistance(px, py, data.Location.x, data.Location.y);
                        }
                    }
                    if (nearest > centerDist) {
                        nearest = centerDist;
                        tempAddr = ObjectPointer;
                    }
                }
            }
        }
            // ??????
        else if (isContain(update_data[i].ClassPath, "STExtraVehicleBase") ||
                 isContain(update_data[i].ClassPath, "FlightVehicle")) {
            if (!isVehicle)continue;
            if (VehicleCount >= MaxVehicleCount) { continue; }
            Vector3A ObjInfo;
            memoryRead(RootCompont + Offset::CoordinateOffset, &ObjInfo, sizeof(ObjInfo));
            ItemData obj{};
            // ????????????
            //isShow
            bool isShow = worldToScreen(&ObjInfo, &obj.Location);
            // ????????????
            obj.Distance = (float) obj.Location.d;

            if (!isShow || obj.Distance > 500 || obj.Location.x > px * 2 + 200 ||
                obj.Location.x < -200) {
                continue;
            }
            //????????????
            if (GetActorBounds != nullptr && showVehicleBox) {
                FVector origin{0, 0, 0}, extends{0, 0, 0};
                GetActorBounds(reinterpret_cast<void *>(ObjectPointer), true, origin, extends);
                Box3D({ObjInfo.X, ObjInfo.Y, ObjInfo.Z}, {extends.x, extends.y, extends.z});
            }
            uintptr_t vehicleCommonComponent = update_data[i].vehicleCommonComponent;

            float hpInfo[2] = {0};
            memoryRead(vehicleCommonComponent + Offset::vehicleHpOffset - 0x4, hpInfo,
                       sizeof(hpInfo));
            obj.ItemHP = hpInfo[1] / hpInfo[0] * 100;
            float fuelInfo[2] = {0};
            memoryRead(vehicleCommonComponent + Offset::vehicleFuelOffset - 0x4, fuelInfo,
                       sizeof(fuelInfo));
            obj.ItemFuel = fuelInfo[1] / fuelInfo[0] * 100;
            VehicleCount++;
            static TextureInfo textureInfo;

            char *wp_name;
            ImVec4 wp_color;
            if (GetVehicleInfo((char *) update_data[i].ItemName.c_str(), &wp_name, &textureInfo,&wp_color)) {
                Vector4Screen Out = obj.Location;
                if (showImg) {
                    if (!showText) {
                        char Temp[256];
                        sprintf(Temp, "(%dm)", int(obj.Distance));
                        DrawStrokeText(Out.x - 15- 20, Out.y + 45, vehicle_color, Temp);
                    }
                    //     DrawCircleFilled(Out.x + 5, Out.y, 25,ImVec4{255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 40.f / 255.f},50);
                    DrawImage(Out.x - 40, Out.y - 50, textureInfo.w / 4.5, textureInfo.h / 4.5,
                              textureInfo.textureId);
                    if (showVehicleInfo) {
                        ImVec4 HPColor =
                                obj.ItemHP < 80 ? obj.ItemHP < 60 ? obj.ItemHP < 30 ? ImVec4{0.5f,
                                                                                             0.0f,
                                                                                             0.0f,
                                                                                             210 /
                                                                                             255.f}
                                                                                    : ImVec4{1, 0,
                                                                                             0,
                                                                                             210 /
                                                                                             255.f}
                                                                  : ImVec4{1, 1, 0, 210 / 255.f}
                                                : ImVec4{Color.Yellow};
                        DrawRectFilled2(Out.x - 35, Out.y + 15, obj.ItemHP * 120 / 100, 15,
                                        HPColor);
                        {
                            ImVec4 HPColor =
                                    obj.ItemFuel < 80 ? obj.ItemFuel < 60 ? obj.ItemFuel < 30
                                                                            ? ImVec4{0.5f, 0.0f,
                                                                                     0.0f,
                                                                                     210 / 255.f}
                                                                            : ImVec4{1, 0, 0,
                                                                                     210 / 255.f}
                                                                          : ImVec4{1, 1, 0,
                                                                                   210 / 255.f}
                                                      : ImVec4{Color.Green};
                            DrawRectFilled2(Out.x -35, Out.y + 50- 20, obj.ItemFuel * 120 / 100, 15,
                                            HPColor);
                        }
                        DrawRect(Out.x -35, Out.y + 15, 100 * 120 / 100, 30, Color.White, 1);//??????
                    }
                }
                if (showText) {
                    char Temp[256];
                    if (showVehicleInfo) {
                        sprintf(Temp, "%s(%dm)\n??????(%d)\n??????(%d)", wp_name, int(obj.Distance),
                                (int) obj.ItemHP, (int) obj.ItemFuel);
                    } else {
                        sprintf(Temp, "%s(%dm)", wp_name, int(obj.Distance));
                    }
                    int tempNum = 65;
                    if (!showImg) {
                        tempNum = 20;
                    }
                    DrawStrokeText(Out.x - 35, Out.y + tempNum- 20, vehicle_color, Temp);
                }
            }

        }
            // ???????????????->>
        else if (isContain(update_data[i].ClassPath, "PickUpListWrapperActor") ||
                 isContain(update_data[i].ClassPath, "AirDropBoxActor")) {
            if (!showChest && !showDrop)continue;
            if (AirBoxCount >= MaxAirBoxCount) { continue; }
            ItemData obj{};
            Vector3A ObjInfo;
            memoryRead(RootCompont + Offset::CoordinateOffset, &ObjInfo, sizeof(ObjInfo));
            bool isShow = worldToScreen(&ObjInfo, &obj.Location);
            // ????????????
            obj.Distance = (float) obj.Location.d;
            if (!isShow || obj.Distance > 100 || obj.Distance < 0) { continue; }
            float D2Dis = get2dDistance(px, py, obj.Location.x, obj.Location.y);
            if (!(D2Dis < boxRange || dropShowType == 1)) {continue;}
            obj.ItemNum = getI(ObjectPointer + Offset::IDNumberOffset); //??????????????????

            if (obj.ItemNum < 50 && obj.ItemNum > 0) {
                int z = 0;
                for (int j = 0; j < obj.ItemNum; j++) {
                    uintptr_t BoxAddress = getA(ObjectPointer + Offset::BoxAddressOffset) +j * Offset::BoxItemSize; //????????????
                    int ItemID = getI(BoxAddress + 0x4); //??????ID
                    if (ItemID > 100000 && ItemID < 999999) {
                        string itemName = getBoxName(ItemID);
                        if (isContain(itemName, "Error")){continue;}
                        if (isContain(update_data[i].ItemName, "AirDropListWrapperActor") ||
                            isContain(update_data[i].ItemName, "AirDrop") && obj.Distance < 3000 &&showDrop) {
                                z++;
                                DrawStrokeText(obj.Location.x, obj.Location.y - z * getFontHeight(),arr[ItemID % length], itemName.c_str());
                                char countTemp[256];
                                sprintf(countTemp, "%s(%d)", "??????", (int) obj.Distance);
                                DrawStrokeText(obj.Location.x, obj.Location.y, Color.White,countTemp);

                        } else if ((isContain(update_data[i].ItemName, "PickUpListWrapperActor") ||
                                    isContain(update_data[i].ItemName, "PlayerDeadInventoryBox_C")) && showChest && obj.Distance < 20) {
                                z++;
                                DrawStrokeText(obj.Location.x, obj.Location.y - z * getFontHeight(),arr[ItemID % length], itemName.c_str());
                                char countTemp[256];
                                sprintf(countTemp, "%s(%d)", "??????", (int) obj.Distance);
                                DrawStrokeText(obj.Location.x, obj.Location.y, Color.White,countTemp);
                        }
                    }
                }
            }
            AirBoxCount++;
        }
            // ??????????????????
        else if (isContain(update_data[i].ClassPath, "PickUpWrapperActor")) {
            if (ItemsCount >= MaxItemsCount) { continue; }
            if (!isItems)continue;
            Vector3A ObjInfo;
            memoryRead(RootCompont + Offset::CoordinateOffset, &ObjInfo, sizeof(ObjInfo));
            ItemData data{};
            bool isShow = worldToScreen(&ObjInfo, &data.Location);
            // ????????????
            data.Distance = (float) data.Location.d;
            if (data.Distance < 3 || data.Distance > 20 || !isShow ||
                data.Location.x > px * 2 + 5 || data.Location.x < 0) { continue; }
            // ????????????
            ItemsCount++;
            char *wp_name;
            ImVec4 wp_color;
            static TextureInfo textureInfo;
            if (GetItemInfo((char *) update_data[i].ItemName.c_str(), &wp_name, &textureInfo,
                            &wp_color)) {
                if (showText) {
                    char Temp[256];
                    sprintf(Temp, "%s(%dm)", wp_name, int(data.Distance));
                    DrawStrokeText(data.Location.x - 20, data.Location.y, wp_color, Temp);
                }
                if (showImg) {
                    if (data.Distance <= 20) {
                        DrawImage(data.Location.x - textureInfo.x, data.Location.y - 20,
                                  textureInfo.w, textureInfo.h, textureInfo.textureId);
                    }
                }
            }
        }
            // ???????????????
        else if (isContain(update_data[i].ClassPath, "EliteProjectile")) {
            if (!isMissiles)
                continue;
            if (GrenadeCount >= MaxGrenadeCount) { continue; }
            Vector3A ObjInfo;
            memoryRead(RootCompont + Offset::CoordinateOffset, &ObjInfo, sizeof(ObjInfo));
            ItemData obj{};
            bool isShow = worldToScreen(&ObjInfo, &obj.Location);
            if (!isShow) { continue; }
            // ????????????
            obj.Distance = (float) obj.Location.d;
            if (strstr("ProjSmoke_BP_C", update_data[i].ItemName.c_str())) {
                float projSmoke2D = get2dDistance(px, py, obj.Location.x, obj.Location.y - 150);
                if (projSmoke2D <= projSomoke) {
                    isMyProjSomoke = true;
                }
                if (!isProjSomoke) {
                    isMyProjSomoke = false;
                }
            } else {
                isMyProjSomoke = false;
            }

            //????????????
            char *wp_name;
            ImVec4 wp_color;
            if (GetGrenadeInfo((char *) update_data[i].ItemName.c_str(), &wp_name, &wp_color)) {
                if (strstr("ProjGrenade_BP_C", update_data[i].ItemName.c_str()) ||
                    strstr("ProjBurn_BP_C", update_data[i].ItemName.c_str())) {
                    ImVec4 myGren;
                    if (obj.Distance < 20 && obj.Distance > 5) {
                        DrawStrokeText(px, 190, Color.Red, "?????????????????????,??????!!!");
                        myGren = Color.Yellow;
                    } else if (obj.Distance <= 5) {
                        myGren = Color.Red;
                    } else {
                        myGren = Color.Green;
                    }
                    DrawLine(px, 200, obj.Location.x, obj.Location.y, myGren, lineWidth);
                    if (strstr("ProjGrenade_BP_C", update_data[i].ItemName.c_str())) {
                        DrawCircleFilled(obj.Location.x, obj.Location.y, 40, myGren, 0);
                    } else {
                        DrawRectFilled(obj.Location.x - 25, obj.Location.y - 15,
                                       obj.Location.x + 25, obj.Location.y + 15, myGren);
                    }
                }
                char Temp[256];
                sprintf(Temp, "%s(%dm)", wp_name, int(obj.Distance));
                DrawStrokeText(obj.Location.x, obj.Location.y, wp_color, Temp);
            }
            GrenadeCount++;
        }
    }

    //???????????????????????????,???????????????
    if (aimbot && NetDriver != 0) {
        if (aimData == 0) { aimData = tempAddr; }
        if (isMincore(aimData, 0x3000) == 1 && !isMyProjSomoke) {
            float hp = getF(aimData + Offset::HealthOffset);
            bool isAi = getI(aimData + Offset::isBotOffset);
            bool isPour = !(!dieNoAim && hp == 0); // ??????????????????
            bool isPour1; //??????????????????
            isAi ? isPour1 = aiAim : isPour1 = true;
            // uintptr_t bone =getA(getA(aimData + Offset::BoneAddrOffset) + Offset::BoneOffset) +Offset::perLODBiasTypeInfoOffset;
            // uintptr_t mesh =getA(aimData + Offset::BoneAddrOffset) + Offset::BoneMatrixOffset;
            uintptr_t mesh_ = getA(aimData + Offset::BoneAddrOffset);
            Vector3A ObjInfo = {0, 0, 0};
            //getBoneLocation(mesh, ObjInfo, bone + 5 * 48);
            ObjInfo = getBoneLoc(mesh_, 5);
            Vector3A pOut;
            //????????????
            worldToScreen(&ObjInfo, &pOut
            );
            if (isPour && isPour1 && lineAim) {
                DrawLine(px, py, pOut.X, pOut.Y, Color.Green, 3);
            }
            //????????????????????????????????????
            if ((bIsPressingFireBtn || bIsGunADS) &&
                isPour && isPour1) {
                if (aimData != 0) {
                    if (isMincore(mesh_, Offset::PointerSize) == 1) {
                        if (aimLocation == 0) {
                            //ObjInfo = getBoneLoc(mesh_, 5);
                            bool isVisib = !(!isVisiblePoint(ObjInfo) && !visibilityAim);
                            if (!isVisib) {
                                ObjInfo = {0, 0, 0};
                            }
                        } else if (aimLocation == 1) {
                            //getBoneLocation(mesh, ObjInfo, bone + 4 * 48);
                            ObjInfo = getBoneLoc(mesh_, 4);
                            bool isVisib = !(!isVisiblePoint(ObjInfo) && !visibilityAim);
                            if (!isVisib) {
                                ObjInfo = {0, 0, 0};
                            }
                        } else if (aimLocation == 2) {
                            ObjInfo = getBoneLoc(mesh_, 2);
                            //getBoneLocation(mesh, ObjInfo, bone + 2 * 48);
                            bool isVisib = !(!isVisiblePoint(ObjInfo) && !visibilityAim);
                            if (!isVisib) {
                                ObjInfo = {0, 0, 0};
                            }
                        } else {
                            //????????????
                            ObjInfo = getBoneLoc(mesh_, 5);
                            //getBoneLocation(mesh, ObjInfo, bone + 5 * 48);
                            if (!(isVisiblePoint(ObjInfo) || visibilityAim)) {
                                //????????????
                                ObjInfo = aimBoneLocation(mesh_);
                            }
                        }
                        if (ObjInfo.X == 0 || ObjInfo.Y == 0) {
                            aimData = 0; // ??????????????????
                            isMyProjSomoke = false;
                        } else {
                            setAimLocation(aimData, ObjInfo);
                        }
                    } else {
                        aimData = 0;
                        isMyProjSomoke = false;
                    }
                }
            } else {
                aimData = 0;
                isMyProjSomoke = false;
            }
        } else {
            aimData = 0;
            isMyProjSomoke = false;
        }
    }
    isSend = false;
}








