#include "Structs.h"
#include "Offsets.h"

#include "log.h"

#include <string>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <numeric>
#include "MemoryUtils.h"
#include "GameUtils.h"
#include "Offsets.h"
#define PI 3.1415926
float px = 0, py = 0;

float screenSizeX = 0;
float screenSizeY = 0;

uintptr_t playerCameraManagerBase;


class FVector {
public:
    FVector() : x(0.f), y(0.f), z(0.f) {

    }

    FVector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

    }

    ~FVector() {

    }

    float x;
    float y;
    float z;

    inline float Dot(FVector v) {
        return x * v.x + y * v.y + z * v.z;
    }

    inline float Distance2D(float x1, float y1) {
        return (float) (sqrt(pow(double(x - x1), 2.0) + pow(double(y - y1), 2.0)));
    }

    inline float Distance(FVector v) {
        return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
    }

    inline float Length() {
        return sqrtf(x * x + y * y + z * z);
    }

    inline FVector &operator+=(const FVector &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline FVector &operator-=(const FVector &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline FVector &operator*=(const FVector &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    inline FVector &operator/=(const FVector &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }

    inline FVector &operator+=(float v) {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    inline FVector &operator-=(float v) {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    inline FVector &operator*=(float v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    inline FVector &operator/=(float v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    inline FVector operator-() const {
        return FVector(-x, -y, -z);
    }

    inline FVector operator+(const FVector &v) const {
        return FVector(x + v.x, y + v.y, z + v.z);
    }

    inline FVector operator-(const FVector &v) const {
        return FVector(x - v.x, y - v.y, z - v.z);
    }

    inline FVector operator*(const FVector &v) const {
        return FVector(x * v.x, y * v.y, z * v.z);
    }

    inline FVector operator/(const FVector &v) const {
        return FVector(x / v.x, y / v.y, z / v.z);
    }

    inline FVector operator+(float v) const {
        return FVector(x + v, y + v, z + v);
    }

    inline FVector operator-(float v) const {
        return FVector(x - v, y - v, z - v);
    }

    inline FVector operator*(float v) const {
        return FVector(x * v, y * v, z * v);
    }

    inline FVector operator/(float v) const {
        return FVector(x / v, y / v, z / v);
    }
};

class FRotator {
public:
    FRotator() : Pitch(0.f), Yaw(0.f), Roll(0.f) {

    }

    FRotator(float _Pitch, float _Yaw, float _Roll) : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll) {

    }

    ~FRotator() {

    }

    float Pitch;
    float Yaw;
    float Roll;

    inline FRotator Clamp() {

        if (Pitch > 180) {
            Pitch -= 360;
        } else {
            if (Pitch < -180) {
                Pitch += 360;
            }
        }
        if (Yaw > 180) {
            Yaw -= 360;
        } else {
            if (Yaw < -180) {
                Yaw += 360;
            }
        }
        if (Pitch > 89) {
            Pitch = 89;
        }
        if (Pitch < -89) {
            Pitch = -89;
        }
        while (Yaw < 180) {
            Yaw += 360;
        }
        while (Yaw > 180) {
            Yaw -= 360;
        }
        Roll = 0;
        return FRotator(Pitch, Yaw, Roll);
    }

    inline float Length() {
        return sqrtf(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
    }

    FRotator operator+(FRotator v) {
        return FRotator(Pitch + v.Pitch, Yaw + v.Yaw, Roll + v.Roll);
    }

    FRotator operator-(FRotator v) {
        return FRotator(Pitch - v.Pitch, Yaw - v.Yaw, Roll - v.Roll);
    }
};

class CameraManager {
public:
    static FVector GetLocation() {
        FVector location;
        Vector3A tempLocation;
        memoryRead(playerCameraManagerBase + Offset::dw_CameraCacheEntry + Offset::dw_MinimalViewInfo + Offset::dw_Location,
                 &tempLocation, sizeof(tempLocation));

        location.x = tempLocation.X;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location);
        location.y = tempLocation.Y;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location +0x4);
        location.z = tempLocation.Z;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location +0x8);

        return location;
    }

    static FVector GetRotation() {
        FVector location;

        Vector3A tempLocation;
        memoryRead(playerCameraManagerBase +Offset:: dw_CameraCacheEntry + Offset::dw_MinimalViewInfo + Offset::dw_Rotation,&tempLocation, sizeof(tempLocation));
        location.x = tempLocation.X;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location);
        location.y = tempLocation.Y;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location +0x4);
        location.z = tempLocation.Z;// getF(playerCameraManagerBase + dw_CameraCacheEntry + dw_MinimalViewInfo + dw_Location +0x8);

        return location;
    }

    static float GetFov() {
        return getF(playerCameraManagerBase +Offset:: dw_CameraCacheEntry + Offset::dw_MinimalViewInfo + Offset::dw_FOV);
    }

    static bool WorldToScreen(FVector WorldLocation, FVector *pOut) {
        FVector Rotation = GetRotation();
        FVector vAxisX, vAxisY, vAxisZ;
        GetAxes(Rotation, &vAxisX, &vAxisY, &vAxisZ);
        FVector vDelta = WorldLocation - GetLocation();
        FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
        //LOGD("x %f y:%f z:%f px:%f py:%f",vTransformed.x,vTransformed.y,vTransformed.z,screenSizeX,screenSizeX);

        if (vTransformed.z < 1.f) {
            return 0;
        }
        float FovAngle = GetFov();
    //    LOGD("fov:%f",FovAngle);
        pOut->x = (screenSizeX / 2.0f) +
                  vTransformed.x * ((screenSizeX / 2.0f) / tanf(FovAngle * (float) PI / 360.f)) /
                  vTransformed.z;
        pOut->y = (screenSizeY / 2.0f) -
                  vTransformed.y * ((screenSizeX / 2.0f) / tanf(FovAngle * (float) PI / 360.f)) /
                  vTransformed.z;
        if (pOut->x > 0 && pOut->y > 0) {
            if (pOut->x <= (screenSizeX / 2.0f) * 2.0f) {
                return 1;
            }
        }
        return 0;
    }


private:
    static void GetAxes(FVector Rotation, FVector *X, FVector *Y, FVector *Z) {
        float radPitch = (Rotation.x * float(PI) / 180.f);
        float radYaw = (Rotation.y * float(PI) / 180.f);
        float radRoll = (Rotation.z * float(PI) / 180.f);

        float SP = sinf(radPitch);
        float CP = cosf(radPitch);
        float SY = sinf(radYaw);
        float CY = cosf(radYaw);
        float SR = sinf(radRoll);

        float CR0 = cosf(radRoll);

        X->x = CP * CY;
        X->y = CP * SY;
        X->z = SP;

        Y->x = SR * SP * CY - CR0 * SY;
        Y->y = SR * SP * SY + CR0 * CY;
        Y->z = -SR * CP;

        Z->x = -(CR0 * SP * CY + SR * SY);
        Z->y = CY * SR - CR0 * SP * SY;
        Z->z = CR0 * CP;
    }
};