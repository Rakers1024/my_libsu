#include <imgui.h>
#include <string.h>
#include <string>
#include <Structs.h>

using namespace std;

//
// Created by admin on 2021/11/16.
//

void DrawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size);

void DrawRect(int x, int y, int w, int h, ImVec4 color, int size);

void DrawRectFilled(int x, int y, int w, int h, ImVec4 color);

void DrawCircle(int x, int y, float radius, ImVec4 color, int segments, int thickness);

void DrawRect2(int x, int y, int w, int h, ImVec4 color,
               int size);

void DrawImage(int x, int y, int w, int h, ImTextureID Texture);

void DrawText(int x, int y, ImVec4 color, char *str);

void doDraw();
void
drawRadar(float dis, float RotationAngleX,
          float RotationAngleY, bool isAI, bool isDie,float MapX,float MapY,float RadarLocationX, float RadarLocationY, float angle);


TextureInfo createTexture(char *ImagePath);

TextureInfo createTexture(const string &ImagePath);

bool GetGrenadeInfo(char *gname, char **name, ImVec4 *color);

void DrawStrokeText(int x, int y, ImVec4 color, const char *str);

void DrawCircleFilled(int x, int y, int radius, ImVec4 color, int segments);

void InitTexture();

bool GetItemInfo(char *gname, char **name, TextureInfo *textureId, ImVec4 *color);

bool GetVehicleInfo(char *gname, char **name, TextureInfo *textureId, ImVec4 *color);

bool GetWeaponsInfo(int id, char **name, TextureInfo *textureInfo);

string getVehicleName(char *ObjName);

string getItemName(char *s);

void DrawRectFilled2(int x, int y, int w, int h, ImVec4 color);
void drawWarning(float x, float y, int dis);
void DrawBoxLine(Vector2A origin, Vector2A dest, ImVec4 color, int size);

string getBoxName(int id);
void DrawInfo(Rect box, float entityHealth, int TeamID, char *name, float d, bool isRat, bool isAi,
              int wqID, bool vis, bool Die, PlayerData obj);
/*************************** 绘制配置 ***********************************/
extern bool showHealth, showLine, showCrosshair, showDis, showInfo, showRadar;
extern bool showBone, showBox;
extern bool showItem, show556, show762, showRifle, showMirror, showExpansion, showDrug, showArmor, showSubmachine, showSniper, showOtherParts;
extern bool showVehicle, showGrenade, showWeapons, isVisibility;
extern bool showChest, showDrop, showVehicleInfo, isObjAimed;
extern bool showTouzhi, showSandan, isSelectedAim, showVehicleBox;
extern bool aimShortDis;
extern bool showRange;
extern bool showImg, showText;
extern bool showWarning, showTarget;

extern float radarLocation;
extern int espWidth, boneWidth, boxWidth, lineWidth;
extern int boxType;
extern bool aimFire, aimOpen;
extern int boxRange;
extern int boxShowType, dropShowType;
extern bool showBoxRange, isRunningLoc;
extern int touchMode;
extern ImVec4 selectedAim_color, bone_color, box_color, line_color, fallen_color
, visibility_color, ai_color, health_color
, drop_color
, aim_color
//药品
, drug_color
//背敌预警颜色
, warning_color
//载具颜色
, vehicle_color
//装备颜色
, armor_color
//狙击枪颜色
, snipe_color
//子弹颜色
, color_bullet
//冲锋枪颜色
, submachine_color
//投掷物颜色
, missile_color
//步枪
, rifle_color
//散弹枪颜色
, color_shot
//倍镜颜色
, mirror_color
//配件颜色
, expansion_color
//其他配件颜色
, otherparts_color;
//雷达
extern bool showRadarDis;
extern int radarOffsetX ;
extern int radarOffsetY ;
extern int radarType;
extern bool visibilityAim, dieNoAim, lineAim, aiAim, isDlip, isShowLoc, aimbot;
extern int aimLocation, aimType, aimedMode, aimChoose;
extern float aimRange, touchJitter, touchSpeed, touchPress, touchLocX, touchLocY, aimPress, aimSpeed, aimAnticipation;
extern float max_shake;
extern float SlideRanges;
extern bool isShowMenu;
extern float draw_scale;

int getFontHeight();
void initDraw();
void DrawNum(int aiNum, int playerNum);

//烟雾不瞄
extern float projSomoke;
extern bool isProjSomoke, isProjSomokeRange;
/*************************** 绘制配置 ***********************************/
