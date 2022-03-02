#include <cstdint>
#include <string>
#include "Structs.h"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <cmath>
#include <dirent.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <string>

#include <imgui.h>
#include <string.h>
#include <string>
#include <GLES3/gl3.h>
#include <tgmath.h>
#include <imgui_internal.h>
#include <dirent.h>

std::string getUEString(uintptr_t address);
std::string GetFNameFromID(unsigned int index,uintptr_t GnameAddress);
std::string getName(uintptr_t actor,uintptr_t GnameAddress);
void getClassName(uintptr_t obj, char *name,uintptr_t GnameAddress);
std::string GetFNameFromID(unsigned int index,uintptr_t GnameAddress);
std::string getName(uintptr_t actor,uintptr_t GnameAddress);
void getClassName(uintptr_t obj, char *name,uintptr_t GnameAddress);
std::string getUEString(uintptr_t address);
float getDistance(float zx, float zy, float zz, float dx, float dy, float dz);
bool isContain(std::string str, const char *check);
Vector2A rotateCoord(float angle, float objectRadar_x, float objectRadar_y);
struct Vector2A World2ScreenMain(struct D3DMatrix viewMatrix, struct Vector3A pos);
void
ToMatrixWithScale(Vector4A Rotation, Vector3A Translation, Vector3A Scale3D, D3DMatrix &result);
void getPlayerName(UTF8 *buf, long int nameOffset);
[[noreturn]] void *updateDataList(void *);
void createDataList() ;
