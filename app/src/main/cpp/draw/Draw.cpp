//
// Created by admin on 2021/11/16.
//

#include "Draw.h"
#include <jni.h>
#include <android/log.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/system_properties.h>
#include <string>
#include "../include/log.h"
#include  <sys/mman.h>
#include <imgui.h>
#include <string.h>
#include <string>
#include <GLES3/gl3.h>
#include <tgmath.h>
#include <imgui_internal.h>
#include <dirent.h>
#include <stb_image.h>


using namespace std;
extern float radarSize;
extern bool showRadarBox;
extern string initPath;
bool isContain(const string &str, const char *check) {
    size_t found = str.find(check);
    return (found != string::npos);
}


static TextureInfo textureInfo;

TextureInfo createTexture(char *ImagePath) {
    int w, h, n;
    stbi_uc *data = stbi_load(ImagePath, &w, &h, &n, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (n == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    stbi_image_free(data);
    textureInfo.textureId = (GLuint *) texture;
    textureInfo.w = w;//* 1.5f;
    textureInfo.h = h;//* 1.5f;
    return textureInfo;
}


TextureInfo createTexture(const string &ImagePath) {
    int w, h, n;
    stbi_uc *data = stbi_load(ImagePath.c_str(), &w, &h, &n, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // if (n == 3) {
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //}
    stbi_image_free(data);
    textureInfo.textureId = (GLuint *) texture;
    textureInfo.w = (int) (w);
    textureInfo.h = (int) (h);
    return textureInfo;
}

//获取绘制文字宽度
float calcTextSize(const char *text, float font_size) {
    ImGuiContext &g = *GImGui;

    ImFont *font = g.Font;

    ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, -1.0f, text, NULL, NULL);

    // Round
    // FIXME: This has been here since Dec 2015 (7b0bf230) but down the line we want this out.
    // FIXME: Investigate using ceilf or e.g.
    // - https://git.musl-libc.org/cgit/musl/tree/src/math/ceilf.c
    // - https://embarkstudios.github.io/rust-gpu/api/src/libm/math/ceilf.rs.html
    text_size.x = IM_FLOOR(text_size.x + 0.99999f);

    return text_size.x;
}


//获取屏幕中心距离
float getD2D(float x, float y, float x1, float y1) {
    float xx1 = x - x1;
    float yy2 = y - y1;
    // 取平方根
    return sqrt(xx1 * xx1 + yy2 * yy2);
}


void DrawCircleFilled(int x, int y, int radius, ImVec4 color, int segments) {
    ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius,
                                                 ImGui::ColorConvertFloat4ToU32(color), segments);
}

void DrawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size) {
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2),
                                         ImGui::ColorConvertFloat4ToU32(color), size);
}

static bool isVisible(float z) {
    //  LogUtils.D("GameInject",z+"");
    return z != 0;
}


void DrawRect(int x, int y, int w, int h, ImVec4 color,
              int size) {  //rounding 方框边缘曲率  //rounding_corners_flags 方框边缘弯曲类型  1.ImDrawCornerFlags_All 2.ImDrawCornerFlags_Top 3. ImDrawCornerFlags_Bot 4. ImDrawCornerFlags_Left 5. ImDrawCornerFlags_Right
    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h),
                                         ImGui::ColorConvertFloat4ToU32(color), 0, 0, size);
}

void DrawRect2(int x, int y, int w, int h, ImVec4 color,
               int size) {  //rounding 方框边缘曲率  //rounding_corners_flags 方框边缘弯曲类型  1.ImDrawCornerFlags_All 2.ImDrawCornerFlags_Top 3. ImDrawCornerFlags_Bot 4. ImDrawCornerFlags_Left 5. ImDrawCornerFlags_Right
    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h),
                                         ImGui::ColorConvertFloat4ToU32(color), 0, 0, size);
}

void DrawRectFilled(int x, int y, int w, int h,
                    ImVec4 color) {  //rounding 方框边缘曲率  //rounding_corners_flags 方框边缘弯曲类型  1.ImDrawCornerFlags_All 2.ImDrawCornerFlags_Top 3. ImDrawCornerFlags_Bot 4. ImDrawCornerFlags_Left 5. ImDrawCornerFlags_Right
    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(w, h),
                                               ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void DrawCircle(int x, int y, float radius, ImVec4 color, int segments, int thickness) {
    ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius,
                                           ImGui::ColorConvertFloat4ToU32(color), segments,
                                           thickness);
}

void DrawImage(int x, int y, int w, int h, ImTextureID Texture) {
    ImGui::GetOverlayDrawList()->AddImage(Texture, ImVec2(x, y), ImVec2(x + w, y + h));
}

void DrawText(char *str, int x, int y, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

void DrawText(const string &str, int x, int y, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color),
                                         str.c_str());
}

/**
 *
 * @param dis
 * @param RadarLocationX
 * @param RadarLocationY
 * @param angle
 * @param isAi
 * @param teamID
 * @param isDie
 * @param RotationAngleX
 * @param RotationAngleY
 * 雷达
 */
static void DrawRadar(float Angle, float x, float y, ImVec4 radar_color) {
    float w = 45.0f / 2 / 1.5f;
    float b = 22.5f;
    // ImVec4 radar_color = Color.White;

    if (Angle <= b || Angle >= 360 - b) {
        DrawLine(x - w, y + w, x, y - w, radar_color, 3);
        DrawLine(x, y - w, x + w, y + w, radar_color, 3);
    } else if (Angle >= 90 - b && Angle <= 90 + b) {
        DrawLine(x - w, y - w, x + w, y, radar_color, 3);
        DrawLine(x + w, y, x - w, y + w, radar_color, 3);
    } else if (Angle >= 180 - b && Angle <= 180 + b) {
        DrawLine(x - w, y - w, x, y + w, radar_color, 3);
        DrawLine(x, y + w, x + w, y - w, radar_color, 3);
    } else if (Angle >= 270 - b && Angle <= 270 + b) {
        DrawLine(x + w, y - w, x - w, y, radar_color, 3);
        DrawLine(x - w, y, x + w, y + w, radar_color, 3);
    } else if (Angle >= 45 - b && Angle <= 45 + b) {
        DrawLine(x + w, y - w, x - w, y, radar_color, 3);
        DrawLine(x + w, y - w, x, y + w, radar_color, 3);
    } else if (Angle >= 135 - b && Angle <= 135 + b) {
        DrawLine(x + w, y + w, x - w, y, radar_color, 3);
        DrawLine(x + w, y + w, x, y - w, radar_color, 3);
    } else if (Angle >= 225 - b && Angle <= 225 + b) {
        DrawLine(x - w, y + w, x, y - w, radar_color, 3);
        DrawLine(x - w, y + w, x + w, y, radar_color, 3);
    } else if (Angle >= 315 - b && Angle <= 315 + b) {
        DrawLine(x - w, y - w, x + w, y, radar_color, 3);
        DrawLine(x - w, y - w, x, y + w, radar_color, 3);
    }

}


void DrawStrokeText(int x, int y, ImVec4 color, const char *str) {
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(
            ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(
            ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(
            ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(
            ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ImVec4 Color, int T) {
    ImGui::GetOverlayDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                                                   ImGui::ColorConvertFloat4ToU32(Color));
}

bool GetGrenadeInfo(char *gname, char **name, ImVec4 *color) {
    if (strstr(gname, "ProjGrenade_BP_C") != nullptr)//手雷
    {
        *name = "小心手雷";
        *color = ImVec4{255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 255.f / 255.f};
        return true;
    }
    if (strstr(gname, "ProjSmoke_BP_C") != nullptr) {
        *name = "有人投掷烟雾";
        *color = ImVec4{255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 255.f / 255.f};
        return true;
    }
    if (strstr(gname, "ProjBurn_BP_C") != nullptr) {
        *name = "小心燃烧瓶";
        *color = ImVec4{255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 255.f / 255.f};
        return true;
    }
    if (strstr(gname, "BP_Grenade_Pan_C") != nullptr) {
        *name = "小心平底锅";
        *color = ImVec4{255.f / 255.f, 0.f / 255.f, 0.f / 255.f, 255.f / 255.f};
        return true;
    }
    return false;
}

void DrawRectFilled2(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void DrawBoxLine(Vector2A origin, Vector2A dest, ImVec4 color, int size) {
    DrawLine(origin.X, origin.Y, dest.X, dest.Y, color, size);
}

void DrawText(int x, int y, ImVec4 color, float font_size, char *str) {
    int start_len = calcTextSize("15.网网网网网网网", font_size);
    int end_len = calcTextSize(str, font_size);

    /*
    int str_len = calcTextSize( "文字绘制测试",40.0f);
    ImGui::GetOverlayDrawList()->AddRect({200, 324}, {200 + (float)str_len, 556}, ImColor(255, 255, 255), 0.0f, 0, 2.0f);
    int four_len = calcTextSize( "文字居左",40.0f);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(200, 324), ImColor(255, 255, 255), "文字绘制测试");
    ImGui::GetOverlayDrawList()->AddText(ImVec2(200, 388), ImColor(255, 255, 255), "文字居左");
    ImGui::GetOverlayDrawList()->AddText(ImVec2(200 + str_len / 2 - four_len / 2, 452), ImColor(255, 255, 255), "文字居中");
    ImGui::GetOverlayDrawList()->AddText(ImVec2(200 + str_len - four_len, 516), ImColor(255, 255, 255), "文字居右");

    */

    ImGui::GetOverlayDrawList()->AddText(ImVec2(x + start_len / 2 - end_len / 2, y),
                                         ImGui::ColorConvertFloat4ToU32(color), str);

}

extern int glWidth, glHeight;
extern int screenWidth;

int getFontHeight() {
    return screenWidth / 100;
}

string getBoxName(int id) {
    if (id == 601006) return "[药品]医疗箱";
    if (id == 601005) return "[药品]急救包";
    if (id == 601004) return "[药品]绷带";
    if (id == 601001) return "[药品]饮料";
    if (id == 601002) return "[药品]强力针";
    if (id == 601003) return "[药品]止痛药";


    if (id == 503001) return "[防具]一级防弹衣";
    if (id == 503002) return "[防具]二级防弹衣";
    if (id == 503003) return "[防具]三级防弹衣";
    if (id == 502001) return "[防具]一级头盔";
    if (id == 502002) return "[防具]二级头盔";
    if (id == 502003) return "[防具]三级头盔";
    if (id == 501001) return "[背包]一级包";
    if (id == 501002) return "[背包]二级包";
    if (id == 501006) return "[背包]三级包";


    if (id == 107001) return "[武器]十字弩";
    if (id == 103003) return "[武器]AWM";
    if (id == 103001) return "[武器]Kar98K";
    if (id == 105002) return "[武器]DP28";
    if (id == 103002) return "[武器]MK14";
    if (id == 101005) return "[武器]狗砸";
    if (id == 101001) return "[武器]AKM";
    if (id == 106005) return "[武器]R45";
    if (id == 101009) return "[武器]MK47";
    if (id == 101006) return "[武器]AUG";
    if (id == 103006) return "[武器]Mini14";
    if (id == 101002) return "[武器]M16A4";
    if (id == 101003) return "[武器]SCAR";
    if (id == 102001) return "[武器]UZI";
    if (id == 102004) return "[武器]汤姆逊冲锋枪";
    if (id == 102003) return "[武器]维克托";
    if (id == 102002) return "[武器]UMP9";
    if (id == 103005) return "[武器]滋水枪";
    if (id == 103008) return "[武器]Win94";
    if (id == 103009) return "[武器]SLR";
    if (id == 105001) return "[武器]QBU";
    if (id == 101007) return "[武器]QBZ";
    //if(id==105001,return "[武器]大菠萝";
    if (id == 106003) return "[武器]R1895";
    if (id == 101004) return "[武器]M416";
    if (id == 106006) return "[武器]短管散弹枪";
    if (id == 104003) return "[武器]S12K";
    if (id == 104002) return "[武器]S1897";
    if (id == 104001) return "[武器]双管猎枪";


    if (id == 306001) return "[子弹]马格南";
    if (id == 302001) return "[子弹]762MM";
    if (id == 303001) return "[子弹]5.56MM";
    if (id == 301001) return "[子弹]9MM";
    if (id == 304001) return "[子弹]12口径";
    if (id == 305001) return "[子弹]45口径";
    if (id == 308001) return "[子弹]信号枪";
    if (id == 307001) return "[子弹]弩箭";


    if (id == 203001) return "[倍镜]红点";
    if (id == 203002) return "[倍镜]全息";
    if (id == 203003) return "[倍镜]2倍镜";
    if (id == 203014) return "[倍镜]3倍镜";
    if (id == 203004) return "[倍镜]4倍镜";
    if (id == 203015) return "[倍镜]6倍镜";
    if (id == 203005) return "[倍镜]8倍镜";


    if (id == 204014) return "[配件]子弹袋";
    if (id == 205004) return "[配件]箭袋";
    if (id == 204010) return "[配件]子弹袋";
    if (id == 202007) return "[配件]激光瞄准器";
    if (id == 202004) return "[配件]拇指握把";
    if (id == 202005) return "[配件]半截握把";
    if (id == 205001) return "[配件]垂直握把";
    if (id == 205003) return "[配件]狙击枪托";
    if (id == 205002) return "[配件]步枪枪托";
    //if(id==205001,return "[配件]枪托";
    if (id == 201003) return "[配件]狙击枪补偿器";
    if (id == 201005) return "[配件]狙击枪消焰器";
    if (id == 201007) return "[配件]狙击枪消音器";
    if (id == 201011) return "[配件]步枪消音器";
    if (id == 201009) return "[配件]步枪补偿器";
    if (id == 201010) return "[配件]步枪消焰器";
    if (id == 201006) return "[配件]冲锋枪消音器";
    if (id == 201004) return "[配件]冲锋枪消焰器";
    if (id == 201002) return "[配件]冲锋枪消焰器";
    if (id == 204009) return "[配件]狙击枪快速扩";
    if (id == 204007) return "[配件]狙击枪扩容";
    if (id == 204008) return "[配件]狙击枪快速";
    if (id == 204013) return "[配件]步枪快速扩容";
    if (id == 204011) return "[配件]步枪扩容";
    if (id == 204012) return "[配件]步枪快速";
    if (id == 204006) return "[配件]冲锋枪快速扩容";
    if (id == 204004) return "[配件]冲锋枪扩容";
    if (id == 204005) return "[配件]冲锋枪快速";
    if (id == 204003) return "[配件]手枪快扩";
    if (id == 204002) return "[配件]手枪快速";
    if (id == 204001) return "[配件]手枪扩容";


    if (id == 602003) return "[投掷]燃烧瓶";
    if (id == 602002) return "[投掷]烟雾弹";
    if (id == 602001) return "[投掷]手雷";
    if (id == 603001) return "[汽油]汽油";
    if (id == 403990) return "[衣服]吉利服";
    if (id == 403187) return "[衣服]吉利服";
    return "Error";
}

extern bool openAccumulation;
extern float px, py;

void DrawText4(char *str, int x, int y, ImVec4 color) {
    ImGui::GetWindowDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

void DrawCircleFilled4(int x, int y, int radius, ImVec4 color, int segments) {
    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(x, y), radius,
                                                ImGui::ColorConvertFloat4ToU32(color), segments);
}

void DrawCircle4(int x, int y, float radius, ImVec4 color, int segments, int thickness) {
    ImGui::GetWindowDrawList()->AddCircle(ImVec2(x, y), radius,
                                          ImGui::ColorConvertFloat4ToU32(color), segments,
                                          thickness);
}

void
drawRadar(float dis, float RotationAngleX,
          float RotationAngleY, bool isAI, bool isDie, float MapX, float MapY, float RadarLocationX,
          float RadarLocationY, float angle) {
    // 绘制雷达
    if (!showRadar) {
        return;
    }

    if (dis > 0 && dis < 450) {

        float Offset_Maps[2] = {0, 0};

        Offset_Maps[0] = MapX;
        Offset_Maps[1] = MapY;

        ImVec4 radar_colir;
        if (isDie) {
            radar_colir = Color.White;
        } else if (!isAI) {
            radar_colir = Color.Red;
        } else {
            radar_colir = Color.Green;
        }
        if (radarType == 1) {
            float proportion = radarSize / 100;
            float MapSize = round(265 * proportion) / 2;

            // Vector2A RadarLocation = player.getRadarLocation();
            float RadarLocation_X = RadarLocationX / (62.5f * 2.5f / proportion);
            float RadarLocation_Y = RadarLocationY / (62.5f * 2.5f / proportion);

            if (showRadarBox) {
                float my_height = MapSize;
                float my_width = MapSize;
                float off = MapSize / 2;

                DrawLine(0 + Offset_Maps[0] - off, 0 + Offset_Maps[1] - off,
                         my_width + Offset_Maps[0] - off,
                         my_height + Offset_Maps[1] - off, Color.Yellow, 1);

                DrawLine(my_width + Offset_Maps[0] - off, 0 + Offset_Maps[1] - off,
                         0 + Offset_Maps[0] - off,
                         my_height + Offset_Maps[1] - off, Color.Yellow, 1);

                DrawRect2(0 + Offset_Maps[0] - off, 0 + Offset_Maps[1] - off,
                          my_width + Offset_Maps[0] - off,
                          my_height + Offset_Maps[1] - off, Color.Black, 1);

                DrawRect2(my_width / 2 - 6 + Offset_Maps[0] - off,
                          my_height / 2 - 6 + Offset_Maps[1] - off,
                          my_width / 2 + 6 + Offset_Maps[0] - off,
                          my_height / 2 + 6 + Offset_Maps[1] - off,
                          Color.Black, 1);
            }

            if (RadarLocation_X < (-MapSize / 2.0f + 45.0f / 2.0f) ||
                RadarLocation_Y < (-MapSize / 2.0f + 45.0f / 2.0f) ||
                RadarLocation_X > (MapSize / 2.0f - 45.0f / 2.0f) ||
                RadarLocation_Y > (MapSize / 2.0f - 45.0f / 2.0f)) {
                float x1 = abs(RadarLocation_X);
                float y1 = abs(RadarLocation_Y);
                float z1 = max(x1, y1) / ((MapSize / 2) - (45.0f / 2.0f));
                RadarLocation_X = RadarLocation_X / z1;
                RadarLocation_Y = RadarLocation_Y / z1;
            }
            DrawRadar(angle, Offset_Maps[0] + RadarLocation_X, Offset_Maps[1] + RadarLocation_Y,
                      radar_colir);
            DrawRect2(Offset_Maps[0] + RadarLocation_X - 15.0f / 2.0f,
                      Offset_Maps[1] + RadarLocation_Y - 15.0f / 2.0f,
                      Offset_Maps[0] + RadarLocation_X + 15.0f / 2.0f,
                      Offset_Maps[1] + RadarLocation_Y + 15.0f / 2.0f, Color.White, 3);
            if (showRadarDis) {
                string temp = (dis < 100 ? " " : "") + to_string((int) ceil(dis));

                DrawText(temp,
                         Offset_Maps[0] + RadarLocation_X - 10.0f / 2.0f,
                         Offset_Maps[1] + RadarLocation_Y + 50.0f / 2.0f * 0.5f, Color.White);
            }
        } else {
            // if (ImGui::Begin("oodff", nullptr,ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoCollapse)) {
            ///  ImGui::SetNextWindowSize({(float )glWidth, (float )glHeight});
            //  ImGui::Begin("mushroom", nullptr, ImGuiWindowFlags_NoBackground| ImGuiWindowFlags_NoTitleBar |ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove);

            //ImGui::SetWindowPos(ImVec2(0, 0));
            float ox = 0;//ImGui::GetWindowPos().x;
            float oy = 0;//ImGui::GetWindowPos().y;
            //ImGui::Image(imageButton.textureId, ImVec2{72, 72}, ImVec2{0, 0}, ImVec2{1, 1});
            DrawCircle(Offset_Maps[0] + ox, Offset_Maps[1]+oy, 140, Color.White, 0, 3);
            DrawCircle(Offset_Maps[0]+ ox, Offset_Maps[1]+oy, 30, Color.White, 0, 3);
            if (showRadarDis) {
                string temp = to_string((int) dis) + "m";
                DrawText((char *) temp.c_str(), RotationAngleX + 40 + Offset_Maps[0]+ ox,RotationAngleY + Offset_Maps[1]+oy, Color.White);
            }

            DrawCircleFilled(RotationAngleX + Offset_Maps[0]+ ox, RotationAngleY + Offset_Maps[1]+oy,
                             10,
                             radar_colir, 0);
            DrawCircleFilled(Offset_Maps[0]+ ox, Offset_Maps[1]+oy, 10, Color.Yellow, 0);
            // }
            //  ImGui::End();


        }
    }
}

void initDraw() {
    //十字准心
    if (openAccumulation && showCrosshair) {
        int radarLeft = px, radarTop = py;
        DrawLine(radarLeft - 40 + 15, radarTop, radarLeft + 40 - 15, radarTop, Color.White, 1);
        DrawLine(radarLeft, radarTop - 40 + 15, radarLeft, radarTop + 40 - 15, Color.White, 1);
    }
    //自瞄范围
    if (showRange && aimbot) {
        DrawCircle((int) px, (int) py, aimRange, Color.Yellow, 0, 1);
    }
    if (isProjSomokeRange && isProjSomoke) {
        //DrawCircle((int) width / 2, (int) height / 2, projSomoke, Color.Red, 0, 3);
        DrawRectFilled(px - projSomoke, py - projSomoke, px + projSomoke,
                       py + projSomoke, Color.Red_);
    }
    //盒子范围
    if (showBoxRange && (boxShowType == 2 || dropShowType == 2) && (showDrop || showChest)) {
        DrawCircle((int) px, (int) py, boxRange, Color.White, 0, 1);
    }
}

extern TextureInfo back1;
extern TextureInfo back2;

void DrawNum(int aiNum, int playerNum) {
    char countTemp[32];
    sprintf(countTemp, "%d", aiNum + playerNum);
    if (aiNum + playerNum == 0) {
        DrawImage(px - 95, 90, 180, 40, back2.textureId);
        DrawStrokeText(px - 18, 100, Color.White, "安全");
    } else {
        DrawImage(px - 100, 90, 180, 40, back1.textureId);
        DrawStrokeText(px - 12, 100, Color.White, countTemp);
    }
    sprintf(countTemp, "玩家:%d   人机:%d", playerNum, aiNum);
    DrawStrokeText(px / 2, 120, Color.White, countTemp);
    // usleep(100);
}


bool getisDie(int state, float health);

void drawBoneLine(Vector3A start, Vector3A end, bool Die, bool isAi, bool isSelected) {
    ImVec4 BC = {0, 0, 0, 0};
    if (isSelected && isSelectedAim) {
        BC = selectedAim_color;
    } else if ((isVisible(start.Z) || isVisible(end.Z)) || !isVisibility) {
        if (Die) {
            BC = fallen_color;
        } else if (isAi) {
            BC = ai_color;
        } else {
            BC = bone_color;
        }
    } else {
        //掩体颜色
        BC = visibility_color;
    }
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(start.X, start.Y), ImVec2(end.X, end.Y),
                                         ImGui::ColorConvertFloat4ToU32(BC), boneWidth);
}

void DrawBox(PlayerData obj, bool vis, bool Die, bool isAi, bool isSelected) {
    if (isRunningLoc) {
        DrawRect2(obj.runningLoc.x - obj.runningLoc.h / 2.5 + 5, obj.runningLoc.y - 10,
                  obj.runningLoc.x + obj.runningLoc.h / 2.5f + 5,
                  obj.runningLoc.y + obj.runningLoc.h + obj.runningLoc.h / 1.5f, Color.Green,
                  boxWidth);
    }
    if (!showBox) {
        return;
    }
    ImVec4 BC = {0, 0, 0, 0};
    if (isSelected && isSelectedAim) {
        BC = selectedAim_color;
    } else if (vis || !isVisibility) {
        if (Die) {
            BC = fallen_color;
        } else if (isAi) {
            BC = ai_color;
        } else {
            BC = box_color;
        }
    } else {
        //掩体颜色
        BC = visibility_color;
    }

    if (boxType == 1) {
        Draw3DBox Box = obj.draw3DBox;
        DrawBoxLine(Box.v1, Box.v2, BC, boxWidth);
        DrawBoxLine(Box.v2, Box.v3, BC, boxWidth);
        DrawBoxLine(Box.v3, Box.v4, BC, boxWidth);
        DrawBoxLine(Box.v4, Box.v1, BC, boxWidth);

        DrawBoxLine(Box.v5, Box.v6, BC, boxWidth);
        DrawBoxLine(Box.v6, Box.v7, BC, boxWidth);
        DrawBoxLine(Box.v7, Box.v8, BC, boxWidth);
        DrawBoxLine(Box.v8, Box.v5, BC, boxWidth);

        DrawBoxLine(Box.v1, Box.v5, BC, boxWidth);
        DrawBoxLine(Box.v2, Box.v6, BC, boxWidth);
        DrawBoxLine(Box.v3, Box.v7, BC, boxWidth);
        DrawBoxLine(Box.v4, Box.v8, BC, boxWidth);

    } else {
        DrawRect2(obj.Location.x - obj.Location.h / 2.5 + 5, obj.Location.y - obj.HeadSize,
                  obj.Location.x + obj.Location.h / 2.5f + 5,
                  obj.Location.y + obj.Location.h + obj.Location.h / 1.5f, BC, boxWidth);

    }

}

//颜色透明度
static float tm2 = 255 / 255.f;
//颜色透明度
static float tm = 255 / 255.f;


static ImVec4 arr2[] = {{144 / 255.f, 238 / 255.f, 144 / 255.f, tm},
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
static int length = sizeof(arr2) / 20;

void DrawBone(PlayerData obj, bool isSelected) {
    if (!showBone) {
        return;
    }
    bool isDie = getisDie(obj.State, obj.Health);
    //绘制骨骼
    BoneData bones = obj.mBoneData;
    //头到脖子
    drawBoneLine(bones.Head, bones.vNeck, isDie, obj.isAI, isSelected);
    //头
    ImVec4 head_color = arr2[obj.TeamID % length];
    if (bones.Head.Z != 1) {
        head_color = visibility_color;
    }
    DrawCircle(bones.Head.X, bones.Head.Y, obj.HeadSize, head_color, 0, boneWidth);
    //DrawCircleFilled(bones.Head.X, bones.Head.Y, obj.HeadSize / 1.5f, head_color, 0);
    drawBoneLine(bones.vNeck, bones.Chest, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Chest, bones.Pelvis, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.vNeck, bones.Left_Shoulder, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.vNeck, bones.Right_Shoulder, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Left_Shoulder, bones.Left_Elbow, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Left_Elbow, bones.Left_Wrist, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Right_Shoulder, bones.Right_Elbow, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Right_Elbow, bones.Right_Wrist, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Pelvis, bones.Left_Thigh, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Pelvis, bones.Right_Thigh, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Left_Thigh, bones.Left_Knee, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Left_Knee, bones.Left_Ankle, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Right_Thigh, bones.Right_Knee, isDie, obj.isAI, isSelected);
    drawBoneLine(bones.Right_Knee, bones.Right_Ankle, isDie, obj.isAI, isSelected);

}

void drawWarning(float x, float y, int dis) {
    float width = glWidth, height = glHeight;
    if (!showGrenade) {
        return;
    }
    if (dis < 20) {
        DrawStrokeText(px - 110, 400, Color.Red, "敌人靠近,快反击!!!");
    }
    //背敌预警

    char Temp[256];
    sprintf(Temp, "%d m", dis);
    if (y > height) {
        if (x > 0 && x < width) {
            DrawRectFilled(x - 50, height - 60, x + 50, height - 5, warning_color);
            //   DrawText(Temp, x - 2, height - 25, Color.Black);
        } else if (x < 0) {
            DrawRectFilled(5, height - 60, 110, height - 5, warning_color);
            //  DrawText(Temp, 60, height - 25, Color.Black);
        } else if (x > width) {
            DrawRectFilled(width - 110, height - 60, width - 10, height - 5, warning_color);
            //   DrawText(Temp, width - 60, height - 25, Color.Black);

        }
    } else if (y < 0) {
        if (x > 0 && x < width) {
            DrawRectFilled(x - 50, 5, x + 50, 60, warning_color);
            // DrawText(Temp, x - 2, 45, Color.Black);
        } else if (x < 0) {
            DrawRectFilled(10, 5, 110, 60, warning_color);
            //  DrawText(Temp, 60, 45, Color.Black);
        } else if (x > width) {
            DrawRectFilled(width - 110, 5, width - 10, 60, warning_color);
            //  DrawText(Temp, width - 60, 45, Color.Black);
        }
    } else if (x - 160 > width) {
        if (y > 0 && y < height) {
            DrawRectFilled(width - 110, y - 25, width - 5, y + 25, warning_color);
            //   DrawText(Temp, width - 60, y + 10, Color.Black);
        }
    } else if (x + 160 < 0) {
        if (y > 0 && y < height) {
            DrawRectFilled(5, y - 25, 110, y + 25, warning_color);
            //   DrawText(Temp, 60, y + 10, Color.Black);
        }
    }
}

extern int myBIsPressingFireBtn;
float gameFov;

float get2dDistance(float x, float y, float x1, float y1);

void DrawInfo(Rect box, float entityHealth, int TeamID, char *name, float d, bool isRat, bool isAi,
              int wqID, bool vis, bool Die, PlayerData obj) {
    float width = glWidth, height = glHeight;

    bool isSelected = false;
    //选中变色
    if (isSelectedAim) {
        float magic_number = (obj.Distance * gameFov);
        float mx = (width / 4) / magic_number;
        float x = obj.Location.x, y = obj.Location.y;
        auto my = (float) ((width / 1.38) / magic_number);
        float top = y - my + (width / 1.7) / magic_number;
        float bottom = y + my + height / 4 / magic_number;
        //DrawRect2(x-mx, top, x+ mx, bottom, Color.Red,1);
        //选中变色
        if ((x - mx) < (width / 2) && (x + mx) > (width / 2)) {
            if ((top) < (height / 2) && (bottom) > (height / 2)) {
                if (myBIsPressingFireBtn)
                    isSelected = true;
            }
        }

    }
    float x = box.x - (140 - box.w) / 2;
    float y = box.y;
    char dis[50];
    sprintf(dis, "%0.fm", d);
    char TeamText[50];
    if (isAi || isRat) {
        sprintf(TeamText, "%d.%s", TeamID, isAi ? "人机" : "内鬼");
    } else {
        sprintf(TeamText, "%d.%s", TeamID, name);
    }
    int four_len = calcTextSize(TeamText, 22.0f);

    ImVec4 HPColor =
            entityHealth < 80 ? entityHealth < 60 ? entityHealth < 30 ? ImVec4{0.5f, 0.0f, 0.0f,
                                                                               health_color.w}
                                                                      : ImVec4{1, 0, 0,
                                                                               health_color.w}
                                                  : ImVec4{1, 1, 0, health_color.w} : health_color;
    tm = 100.f / 255.f;


    int tmpNum = 0;
    if (!showDis) {
        tmpNum = -15;
    }
    //血量
    if (showHealth) {
        if (boxType == 2) {

            float y = (obj.Location.y + obj.Location.h + obj.Location.h / 1.5f);
            float healthLength = y - (obj.Location.y - obj.HeadSize);
            DrawRectFilled2(obj.Location.x - obj.Location.h / 2.5 - 3,
                            obj.Location.y - obj.HeadSize,
                            5, obj.Health * healthLength / 100, HPColor);

            DrawRect(obj.Location.x - obj.Location.h / 2.5 - 3, obj.Location.y - obj.HeadSize,
                     5, healthLength, Color.Black, 1);

        } else {
            DrawRectFilled2(x - four_len / 5 + 18, y - 45 - tmpNum,
                            entityHealth * (130 + four_len / 5) / 100, 25,
                            HPColor);//血
            DrawRect(x - four_len / 5 + 18, y - 45 - tmpNum, 100 * (130 + four_len / 5) / 100, 25,
                     Color.Black, 1);//血框
        }

    }

    //信息
    if (showInfo) {
        if (boxType == 2) {
            DrawText(x - four_len / 5 + 18, y - 22 - obj.HeadSize, Color.White, 22, TeamText);
        } else {
            DrawText(x - four_len / 5 + 18, y - 44 - tmpNum, Color.White, 22, TeamText);
        }
    }


    //被瞄预警
    if (isObjAimed) {
        if (obj.objAim.X <= 4 && obj.objAim.Y <= 4) {
            DrawStrokeText(px - strlen("警告!!!您被敌人瞄准!!!") * 2 - 50, py - 230,
                           ImVec4(255.f / 255.f, 0, 0, 1.f), "警告!!!您被敌人瞄准!!!");
            DrawLine(px, py - 50, obj.Location.x, obj.Location.y, Color.Red, 3);
        }
    }
    //武器
    if (showWeapons) {
        char *wp_name;
        ImVec4 wp_color;
        TextureInfo textureInfo;
        if (GetWeaponsInfo(wqID, &wp_name, &textureInfo)) {
            if (boxType == 2) {
                int tmp = 0;
                if (!showInfo) {
                    tmp = 20;
                }
                DrawImage(box.x + box.w / 2 - textureInfo.w / 2,
                          box.y - 30 - textureInfo.y - 50 + tmp - obj.HeadSize + 30,
                          textureInfo.w, textureInfo.h, textureInfo.textureId);
            } else {
                if (!showInfo && !showHealth) {
                    DrawImage(box.x + box.w / 2 - textureInfo.w / 2,
                              box.y - 42 - textureInfo.y - tmpNum, textureInfo.w, textureInfo.h,
                              textureInfo.textureId);
                } else {
                    DrawImage(box.x + box.w / 2 - textureInfo.w / 2,
                              box.y - 77 - textureInfo.y - tmpNum, textureInfo.w, textureInfo.h,
                              textureInfo.textureId);
                }
            }
        }
    }
    //绘制射线
    if (showLine) {
        ImVec4 LC = {0, 0, 0, 0};
        if (isSelected && isSelectedAim) {
            LC = selectedAim_color;
        } else if (vis || !isVisibility) {
            if (Die) {
                LC = fallen_color;
            } else if (isAi) {
                LC = ai_color;
            } else {
                LC = line_color;
            }
        } else {
            //掩体颜色
            LC = visibility_color;
        }


        DrawLine(px, 30, obj.Location.x, obj.Location.y, LC, lineWidth);

    }
    //距离.
    if (showDis) {
        if (boxType == 2) {
            DrawStrokeText(x + 52, obj.Location.y + obj.Location.h + obj.Location.h / 1.5f,
                           Color.Yellow, dis);

        } else {
            DrawStrokeText(x + 56, y - 25, Color.Yellow, dis);
        }
    }
    //方框
    DrawBox(obj, vis, Die, isAi, isSelected);
    //骨骼
    DrawBone(obj, isSelected);
}

//载具
struct Vehicle {
    TextureInfo jeep;
    TextureInfo bengbeng;
    TextureInfo jiaoche;
    TextureInfo paoche;
    TextureInfo mianyang;
    TextureInfo yueye;
    TextureInfo kuaiting;
    TextureInfo luonipika;
    TextureInfo daba;
    TextureInfo motuoting;
    TextureInfo xuedimotuo;
    TextureInfo sanlunmotuo;
    TextureInfo motuo;
    TextureInfo wupengpika;
    TextureInfo youpengpika;
    TextureInfo sanlunche;
    TextureInfo xueqiao;
    TextureInfo zhuangjiache;
    TextureInfo huaxiaji;


} vehicle;
//倍镜
struct Part {
    TextureInfo erbei;
    TextureInfo sanbei;
    TextureInfo sibei;
    TextureInfo liubei;
    TextureInfo babei;
    TextureInfo cemiao;
    TextureInfo hongdian;
    TextureInfo quanxi;
} part;
//步枪
static struct Rifle {
    TextureInfo AK;
    TextureInfo AUG;
    TextureInfo DP28;
    TextureInfo G36C;
    TextureInfo GROZA;
    TextureInfo M16A4;
    TextureInfo M249;
    TextureInfo M416;
    TextureInfo M762;
    TextureInfo QBU;
    TextureInfo QBZ;
    TextureInfo SCAR;
    TextureInfo xinhaoqiang;
} rifle;
//刀
static struct Knife {
    TextureInfo guo;
    TextureInfo kandao;
    TextureInfo liandao;
    TextureInfo qiaogun;
    TextureInfo nu;
} knife;
//冲锋枪
static struct charge {
    TextureInfo UMP45;
    TextureInfo VECTOR;
    TextureInfo TANGMUXUN;
    TextureInfo YENIU;
    TextureInfo VSS;
    TextureInfo UZI;
} charge;
//狙击枪
static struct Sniper {
    TextureInfo AWM;
    TextureInfo K98;
    TextureInfo M24;
    TextureInfo MINI14;
    TextureInfo MK14;
    TextureInfo MK47;
    TextureInfo SKS;
    TextureInfo SLR;
    TextureInfo WIN94;
} Sniper;
//子弹
static struct Bullet {
    TextureInfo ZD_45;
    TextureInfo ZD_AWM;
    TextureInfo ZD_556;
    TextureInfo ZD_762;
    TextureInfo ZD_9;
    TextureInfo ZD_12;
    TextureInfo GJ;
} bullet;
//投掷物
static struct missile {
    TextureInfo shoulei;
    TextureInfo ranshaoping;
    TextureInfo yanwudan;
    TextureInfo zhenbaodan;
} missile;
//散弹抢
static struct shot {
    TextureInfo dbs;
    TextureInfo sk12;
    TextureInfo s686;
    TextureInfo s1897;
} shot;
//药
static struct drug {
    TextureInfo bengdai;
    TextureInfo jijiubao;
    TextureInfo yinliao;
    TextureInfo yiliaoxiang;
    TextureInfo shenshangxiansu;
    TextureInfo zhitengyao;
} drug;
//头盔、防弹衣、背包
static struct Helmet {
    TextureInfo sanjitou;
    TextureInfo sanjijia;
    TextureInfo sanjibao;
    TextureInfo erjitou;
    TextureInfo erjijia;
    TextureInfo erjibao;
    TextureInfo yijitou;
    TextureInfo yijijia;
    TextureInfo yijibao;
    TextureInfo jilifu;


} Helmet;
//配件
static struct parts {
    TextureInfo banjiewoba;
    TextureInfo chuizhiwoba;
    TextureInfo jiguangmiaozhun;
    TextureInfo jiandai;
    TextureInfo cf_kuaisudanjia;
    TextureInfo jj_kuaisudanjia;
    TextureInfo sq_kuaisudanjia;
    TextureInfo bq_kuaisudanjia;
    TextureInfo cf_kuaikuo;
    TextureInfo jj_kuaikuo;
    TextureInfo sq_kuaikuo;
    TextureInfo bq_kuaikuo;
    TextureInfo cf_kuorong;
    TextureInfo jj_kuorong;
    TextureInfo sq_kuorong;
    TextureInfo bq_kuorong;
    TextureInfo muzhiwoba;
    TextureInfo cf_buchang;
    TextureInfo jj_buchang;
    TextureInfo bq_buchang;
    TextureInfo uzi_qiangtuo;
    TextureInfo qingxingwoba;
    TextureInfo jj_tuosaiban;
    TextureInfo sd_shoushuqi;
    TextureInfo cf_xiaoyanqi;
    TextureInfo jj_xiaoyanqi;
    TextureInfo bq_xiaoyanqi;
    TextureInfo cf_xiaoyinqi;
    TextureInfo jj_xiaoyinqi;
    TextureInfo sq_xiaoyinqi;
    TextureInfo bq_xiaoyinqi;
    TextureInfo yazuiqiangkou;
    TextureInfo m416_qiangtuo;
    TextureInfo zhijiaowoba;
    TextureInfo k98zidandai;
    TextureInfo sd_zidandai;
} parts;
TextureInfo back1;
TextureInfo back2;


TextureInfo box1;
TextureInfo box2;
static struct Hand {
    TextureInfo K98;
    TextureInfo M416;
    TextureInfo M416A4;
    TextureInfo MK14;
    TextureInfo Grozy;
    TextureInfo Scar;
    TextureInfo M24;
    TextureInfo QBZ;
    TextureInfo DP28;
    TextureInfo MK47;
    TextureInfo S12K;
    TextureInfo S1896;
    TextureInfo SKS;
    TextureInfo SLR;
    TextureInfo M762;
    TextureInfo S686;
    TextureInfo VSS;
    TextureInfo UMP9;
    TextureInfo 手雷;
    TextureInfo 烟雾弹;
    TextureInfo 燃烧瓶;
    TextureInfo 拳头;
    TextureInfo AWM;
    TextureInfo AKM;
    TextureInfo M249;
    TextureInfo QBU;
    TextureInfo Mini14;
    TextureInfo AUG;
    TextureInfo G36C;
    TextureInfo PP19;
    TextureInfo UZI;
    TextureInfo P18C;
    TextureInfo WIN94;
    TextureInfo 汤姆逊;
    TextureInfo Vector;
    TextureInfo 短管散弹;
    TextureInfo 十字弩;
} hand;


void InitTexture() {
    //枪支配件
    parts.banjiewoba = createTexture(initPath +"image/parts1.png");
    parts.chuizhiwoba = createTexture(initPath +"image/parts2.png");
    parts.jiguangmiaozhun = createTexture(initPath +"image/parts3.png");
    parts.jiandai = createTexture(initPath +"image/parts4.png");
    parts.cf_kuaisudanjia = createTexture(initPath +"image/parts5.png");
    parts.jj_kuaisudanjia = createTexture(initPath +"image/parts6.png");
    parts.sq_kuaisudanjia = createTexture(initPath +"image/parts7.png");
    parts.bq_kuaisudanjia = createTexture(initPath +"image/parts8.png");
    parts.cf_kuaikuo = createTexture(initPath +"image/parts9.png");
    parts.jj_kuaikuo = createTexture(initPath +"image/parts10.png");
    parts.sq_kuaikuo = createTexture(initPath +"image/parts11.png");
    parts.bq_kuaikuo = createTexture(initPath +"image/parts12.png");
    parts.cf_kuorong = createTexture(initPath +"image/parts13.png");
    parts.jj_kuorong = createTexture(initPath +"image/parts14.png");
    parts.sq_kuorong = createTexture(initPath +"image/parts15.png");
    parts.bq_kuorong = createTexture(initPath +"image/parts16.png");
    parts.muzhiwoba = createTexture(initPath +"image/parts17.png");
    parts.cf_buchang = createTexture(initPath +"image/parts18.png");
    parts.jj_buchang = createTexture(initPath +"image/parts19.png");
    parts.bq_buchang = createTexture(initPath +"image/parts20.png");
    parts.uzi_qiangtuo = createTexture(initPath +"image/parts21.png");
    parts.qingxingwoba = createTexture(initPath +"image/parts22.png");
    parts.jj_tuosaiban = createTexture(initPath +"image/parts23.png");
    parts.sd_shoushuqi = createTexture(initPath +"image/parts24.png");
    parts.cf_xiaoyanqi = createTexture(initPath +"image/parts25.png");
    parts.jj_xiaoyanqi = createTexture(initPath +"image/parts26.png");
    parts.bq_xiaoyanqi = createTexture(initPath +"image/parts27.png");
    parts.cf_xiaoyinqi = createTexture(initPath +"image/parts28.png");
    parts.jj_xiaoyinqi = createTexture(initPath +"image/parts29.png");
    parts.sq_xiaoyinqi = createTexture(initPath +"image/parts30.png");
    parts.bq_xiaoyinqi = createTexture(initPath +"image/parts31.png");
    parts.yazuiqiangkou = createTexture(initPath +"image/parts32.png");
    parts.m416_qiangtuo = createTexture(initPath +"image/parts33.png");
    parts.zhijiaowoba = createTexture(initPath +"image/parts34.png");
    parts.k98zidandai = createTexture(initPath +"image/parts35.png");
    parts.sd_zidandai = createTexture(initPath +"image/parts36.png");

    //头盔、防弹衣、背包
    Helmet.sanjitou = createTexture(initPath +"image/Helmet1.png");
    Helmet.sanjijia = createTexture(initPath +"image/Helmet2.png");
    Helmet.sanjibao = createTexture(initPath +"image/Helmet3.png");
    Helmet.erjitou = createTexture(initPath +"image/Helmet4.png");
    Helmet.erjijia = createTexture(initPath +"image/Helmet5.png");
    Helmet.erjibao = createTexture(initPath +"image/Helmet6.png");
    Helmet.yijitou = createTexture(initPath +"image/Helmet7.png");
    Helmet.yijijia = createTexture(initPath +"image/Helmet8.png");
    Helmet.yijibao = createTexture(initPath +"image/Helmet9.png");
    Helmet.jilifu = createTexture(initPath +"image/Helmet10.png");
    //药
    drug.bengdai = createTexture(initPath +"image/drug1.png");
    drug.jijiubao = createTexture(initPath +"image/drug2.png");
    drug.yinliao = createTexture(initPath +"image/drug3.png");
    drug.yiliaoxiang = createTexture(initPath +"image/drug4.png");
    drug.shenshangxiansu = createTexture(initPath +"image/drug5.png");
    drug.zhitengyao = createTexture(initPath +"image/drug6.png");
    //投掷物
    missile.shoulei = createTexture(initPath +"image/missile1.png");
    missile.ranshaoping = createTexture(initPath +"image/missile2.png");
    missile.yanwudan = createTexture(initPath +"image/missile3.png");
    missile.zhenbaodan = createTexture(initPath +"image/missile4.png");
    //散弹枪
    shot.dbs = createTexture(initPath +"image/shot1.png");
    shot.sk12 = createTexture(initPath +"image/shot2.png");
    shot.s686 = createTexture(initPath +"image/shot3.png");
    shot.s1897 = createTexture(initPath +"image/shot4.png");
    //载具
    vehicle.jeep = createTexture(initPath +"image/Vehicle1.png");
    vehicle.bengbeng = createTexture(initPath +"image/Vehicle2.png");
    vehicle.jiaoche = createTexture(initPath +"image/Vehicle3.png");
    vehicle.paoche = createTexture(initPath +"image/Vehicle4.png");
    vehicle.mianyang = createTexture(initPath +"image/Vehicle5.png");
    vehicle.yueye = createTexture(initPath +"image/Vehicle6.png");
    vehicle.kuaiting = createTexture(initPath +"image/Vehicle7.png");
    vehicle.luonipika = createTexture(initPath +"image/Vehicle8.png");
    vehicle.daba = createTexture(initPath +"image/Vehicle9.png");
    vehicle.motuoting = createTexture(initPath +"image/Vehicle10.png");
    vehicle.xuedimotuo = createTexture(initPath +"image/Vehicle11.png");
    vehicle.sanlunmotuo = createTexture(
            initPath +"image/Vehicle12.png");
    vehicle.motuo = createTexture(initPath +"image/Vehicle13.png");
    vehicle.wupengpika = createTexture(initPath +"image/Vehicle14.png");
    vehicle.youpengpika = createTexture(
            initPath +"image/Vehicle15.png");
    vehicle.sanlunche = createTexture(initPath +"image/Vehicle16.png");
    vehicle.xueqiao = createTexture(initPath +"image/Vehicle17.png");
    vehicle.zhuangjiache = createTexture(
            initPath +"image/Vehicle18.png");
    vehicle.huaxiaji = createTexture(initPath +"image/Vehicle19.png");


    //倍镜
    part.erbei = createTexture(initPath +"image/part1.png");
    part.sanbei = createTexture(initPath +"image/part2.png");
    part.sibei = createTexture(initPath +"image/part3.png");
    part.liubei = createTexture(initPath +"image/part4.png");
    part.babei = createTexture(initPath +"image/part5.png");
    part.cemiao = createTexture(initPath +"image/part6.png");
    part.hongdian = createTexture(initPath +"image/part7.png");
    part.quanxi = createTexture(initPath +"image/part8.png");
    //步枪
    rifle.AK = createTexture(initPath +"image/Rifle1.png");
    rifle.AUG = createTexture(initPath +"image/Rifle2.png");
    rifle.DP28 = createTexture(initPath +"image/Rifle3.png");
    rifle.G36C = createTexture(initPath +"image/Rifle4.png");
    rifle.GROZA = createTexture(initPath +"image/Rifle5.png");
    rifle.M16A4 = createTexture(initPath +"image/Rifle6.png");
    rifle.M249 = createTexture(initPath +"image/Rifle7.png");
    rifle.M416 = createTexture(initPath +"image/Rifle8.png");
    rifle.M762 = createTexture(initPath +"image/Rifle9.png");
    rifle.QBU = createTexture(initPath +"image/Rifle11.png");
    rifle.QBZ = createTexture(initPath +"image/Rifle12.png");
    rifle.SCAR = createTexture(initPath +"image/Rifle13.png");
    rifle.xinhaoqiang = createTexture(initPath +"image/Rifle14.png");
    //冲锋枪
    charge.UMP45 = createTexture(initPath +"image/charge1.png");
    charge.VECTOR = createTexture(initPath +"image/charge2.png");
    charge.TANGMUXUN = createTexture(initPath +"image/charge3.png");
    charge.YENIU = createTexture(initPath +"image/charge4.png");
    charge.VSS = createTexture(initPath +"image/charge5.png");
    charge.UZI = createTexture(initPath +"image/charge6.png");
    //狙击枪
    Sniper.AWM = createTexture(initPath +"image/Sniper1.png");
    Sniper.K98 = createTexture(initPath +"image/Sniper2.png");
    Sniper.M24 = createTexture(initPath +"image/Sniper3.png");
    Sniper.MINI14 = createTexture(initPath +"image/Sniper4.png");
    Sniper.MK14 = createTexture(initPath +"image/Sniper5.png");
    Sniper.MK47 = createTexture(initPath +"image/Sniper6.png");
    Sniper.SKS = createTexture(initPath +"image/Sniper7.png");
    Sniper.SLR = createTexture(initPath +"image/Sniper8.png");
    Sniper.WIN94 = createTexture(initPath +"image/Sniper9.png");
    //子弹
    bullet.ZD_45 = createTexture(initPath +"image/bullet1.png");
    bullet.ZD_AWM = createTexture(initPath +"image/bullet2.png");
    bullet.ZD_556 = createTexture(initPath +"image/bullet3.png");
    bullet.ZD_762 = createTexture(initPath +"image/bullet4.png");
    bullet.ZD_9 = createTexture(initPath +"image/bullet5.png");
    bullet.ZD_12 = createTexture(initPath +"image/bullet6.png");
    //bullet.GJ = createTexture(initPath +"image/bullet7.png");
    bullet.GJ = createTexture(initPath +"image/parts4.png");


    //刀
    knife.guo = createTexture(initPath +"image/Rifle10.png");
    knife.kandao = createTexture(initPath +"image/Knife1.png");
    knife.liandao = createTexture(initPath +"image/Knife2.png");
    knife.qiaogun = createTexture(initPath +"image/Knife3.png");
    knife.nu = createTexture(initPath +"image/Knife4.png");
    //人数背景
    back1 = createTexture(initPath +"image/back1.png");
    back2 = createTexture(initPath +"image/back2.png");
    //盒子 空投
    box1 = createTexture(initPath +"image/box1.png");
    box2 = createTexture(initPath +"image/box2.png");
    //持枪
    hand.K98 = createTexture(initPath +"image/Hand1.png");
    hand.M416 = createTexture(initPath +"image/Hand2.png");
    hand.M416A4 = createTexture(initPath +"image/Hand3.png");
    hand.MK14 = createTexture(initPath +"image/Hand4.png");
    hand.Grozy = createTexture(initPath +"image/Hand5.png");
    hand.Scar = createTexture(initPath +"image/Hand6.png");
    hand.M24 = createTexture(initPath +"image/Hand7.png");
    hand.QBZ = createTexture(initPath +"image/Hand8.png");
    hand.DP28 = createTexture(initPath +"image/Hand9.png");
    hand.MK47 = createTexture(initPath +"image/Hand10.png");
    hand.S12K = createTexture(initPath +"image/Hand11.png");
    hand.S1896 = createTexture(initPath +"image/Hand12.png");
    hand.SKS = createTexture(initPath +"image/Hand13.png");
    hand.SLR = createTexture(initPath +"image/Hand14.png");
    hand.M762 = createTexture(initPath +"image/Hand15.png");
    hand.S686 = createTexture(initPath +"image/Hand16.png");
    hand.VSS = createTexture(initPath +"image/Hand17.png");
    hand.UMP9 = createTexture(initPath +"image/Hand18.png");
    hand.手雷 = createTexture(initPath +"image/Hand19.png");
    hand.烟雾弹 = createTexture(initPath +"image/Hand20.png");
    hand.燃烧瓶 = createTexture(initPath +"image/Hand21.png");
    hand.拳头 = createTexture(initPath +"image/Hand22.png");
    hand.AWM = createTexture(initPath +"image/Hand23.png");
    hand.AKM = createTexture(initPath +"image/Hand24.png");
    hand.M249 = createTexture(initPath +"image/Hand25.png");
    hand.QBU = createTexture(initPath +"image/Hand26.png");
    hand.Mini14 = createTexture(initPath +"image/Hand27.png");
    hand.AUG = createTexture(initPath +"image/Hand28.png");
    hand.G36C = createTexture(initPath +"image/Hand29.png");
    hand.PP19 = createTexture(initPath +"image/Hand30.png");
    hand.UZI = createTexture(initPath +"image/Hand31.png");
    hand.P18C = createTexture(initPath +"image/Hand32.png");
    hand.WIN94 = createTexture(initPath +"image/Hand33.png");
    hand.汤姆逊 = createTexture(initPath +"image/Hand34.png");
    hand.Vector = createTexture(initPath +"image/Hand35.png");
    hand.短管散弹 = createTexture(initPath +"image/Hand36.png");
    hand.十字弩 = createTexture(initPath +"image/Hand37.png");
}

bool GetItemInfo(char *gname, char **name, TextureInfo *textureId, ImVec4 *color) {
    textureId->x = 10;

    if (show556) {
        if (strcmp(gname, "BP_Ammo_556mm_Pickup_C") == 0) {
            textureId->x = 17;
            textureId->w = bullet.ZD_556.w / 7.5f;
            textureId->h = bullet.ZD_556.h / 7.5f;
            textureId->textureId = bullet.ZD_556.textureId;
            *name = "5.56MM";
            *color = color_bullet;
            return true;
        }
        if (strcmp(gname, "BP_Ammo_762mm_Pickup_C") == 0) {
            textureId->x = 17;
            textureId->w = bullet.ZD_762.w / 7.5f;
            textureId->h = bullet.ZD_762.h / 7.5f;
            textureId->textureId = bullet.ZD_762.textureId;
            *name = "7.62MM";
            *color = color_bullet;
            return true;
        }
        if (strstr(gname, "Ammo_45AC") != nullptr) {
            textureId->x = 17;
            textureId->w = bullet.ZD_45.w / 7.5f;
            textureId->h = bullet.ZD_45.h / 7.5f;
            textureId->textureId = bullet.ZD_45.textureId;
            *name = "45ACP";
            *color = color_bullet;
            return true;
        }
        if (strstr(gname, "Ammo_9mm") != nullptr) {
            textureId->x = 17;
            textureId->w = bullet.ZD_9.w / 7.5f;
            textureId->h = bullet.ZD_9.h / 7.5f;
            textureId->textureId = bullet.ZD_9.textureId;
            *name = "9mm";
            *color = color_bullet;
            return true;
        }
        if (strstr(gname, "Ammo_12Guage") != nullptr) {
            textureId->x = 17;
            textureId->w = bullet.ZD_12.w / 7.5f;
            textureId->h = bullet.ZD_12.h / 7.5f;
            textureId->textureId = bullet.ZD_12.textureId;
            *name = "12 Guage";
            *color = color_bullet;
            return true;
        }
        if (strstr(gname, "Ammo_300Magnum") != nullptr) {
            textureId->x = 17;
            textureId->w = bullet.ZD_AWM.w / 7.5f;
            textureId->h = bullet.ZD_AWM.h / 7.5f;
            textureId->textureId = bullet.ZD_AWM.textureId;
            *name = "300马格南";
            *color = color_bullet;
            return true;
        }
        if (strstr(gname, "Ammo_Bolt") != nullptr) {
            textureId->x = 17;
            textureId->w = bullet.GJ.w / 7.5f;
            textureId->h = bullet.GJ.h / 7.5f;
            textureId->textureId = bullet.GJ.textureId;
            *name = "弩箭";
            *color = color_bullet;
            return true;
        }
    }

    //散弹枪
    if (showSandan) {

        textureId->x = 10;
        float sacle = 6.5f;
        if (strstr(gname, "S12K") != nullptr) {
            textureId->w = shot.sk12.w / sacle;
            textureId->h = shot.sk12.h / sacle;
            textureId->textureId = shot.sk12.textureId;
            *name = "S12K";
            *color = color_shot;
            return true;
        }
        if (strstr(gname, "DBS") != nullptr) {
            textureId->w = shot.dbs.w / sacle;
            textureId->h = shot.dbs.h / sacle;
            textureId->textureId = shot.dbs.textureId;
            *name = "DBS散弹枪";
            *color = color_shot;
            return true;
        }
        if (strstr(gname, "S686") != nullptr) {
            textureId->w = shot.s686.w / sacle;
            textureId->h = shot.s686.h / sacle;
            textureId->textureId = shot.s686.textureId;
            *name = "S686";
            *color = color_shot;
            return true;
        }
        if (strstr(gname, "S1897") != nullptr) {
            textureId->w = shot.s1897.w / sacle;
            textureId->h = shot.s1897.h / sacle;
            textureId->textureId = shot.s1897.textureId;
            *name = "s1897";
            *color = color_shot;
            return true;
        }
        if (strstr(gname, "DP12") != nullptr) {
            textureId->w = shot.dbs.w / sacle;
            textureId->h = shot.dbs.h / sacle;
            textureId->textureId = shot.dbs.textureId;
            *name = "DP12";
            *color = color_shot;
            return true;
        }
    }
    //投掷物
    if (showTouzhi) {
        if (strstr(gname, "Grenade_Stun") != nullptr) {
            textureId->w = missile.zhenbaodan.w / 7.5f;
            textureId->h = missile.zhenbaodan.h / 7.5f;
            textureId->textureId = missile.zhenbaodan.textureId;
            *name = "闪光弹";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Grenade_Shoulei") != nullptr) {
            textureId->w = missile.shoulei.w / 7.5f;
            textureId->h = missile.shoulei.h / 7.5f;
            textureId->textureId = missile.shoulei.textureId;
            *name = "手榴弹";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Grenade_Smoke") != nullptr) {
            textureId->w = missile.yanwudan.w / 7.5f;
            textureId->h = missile.yanwudan.h / 7.5f;
            textureId->textureId = missile.yanwudan.textureId;
            *name = "烟雾弹";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Grenade_Burn") != nullptr) {
            textureId->w = missile.ranshaoping.w / 7.5f;
            textureId->h = missile.ranshaoping.h / 7.5f;
            textureId->textureId = missile.ranshaoping.textureId;
            *name = "燃烧瓶";
            *color = missile_color;
            return true;
        }
        //刀
        if (strcmp(gname, "BP_WEP_Pan_Pickup_C") == 0) {
            textureId->w = knife.guo.w / 7.5f;
            textureId->h = knife.guo.h / 7.5f;
            textureId->textureId = knife.guo.textureId;
            *name = "平底锅";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Sickle") != nullptr) {
            textureId->w = knife.liandao.w / 7.5f;
            textureId->h = knife.liandao.h / 7.5f;
            textureId->textureId = knife.liandao.textureId;
            *name = "镰刀";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Machete") != nullptr) {
            textureId->w = knife.kandao.w / 7.5f;
            textureId->h = knife.kandao.h / 7.5f;
            textureId->textureId = knife.kandao.textureId;
            *name = "砍刀";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "Cowbar") != nullptr) {
            textureId->w = knife.qiaogun.w / 7.5f;
            textureId->h = knife.qiaogun.h / 7.5f;
            textureId->textureId = knife.qiaogun.textureId;
            *name = "铁橇";
            *color = missile_color;
            return true;
        }
        if (strstr(gname, "CrossBow") != nullptr) {
            textureId->w = knife.nu.w / 7.5f;
            textureId->h = knife.nu.h / 7.5f;
            textureId->textureId = knife.nu.textureId;
            *name = "十字弩";
            *color = missile_color;
            return true;
        }

    }

    //显示步枪img
    if (showRifle) {
        textureId->x = 40;
        float sacle = 7.5f;
        if (strcmp(gname, "BP_Pistol_Flaregun") == 0) {
            textureId->w = rifle.xinhaoqiang.w / sacle;
            textureId->h = rifle.xinhaoqiang.h / sacle;
            textureId->textureId = rifle.xinhaoqiang.textureId;
            *name = "信号枪";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_AKM_Wrapper_C") == 0) {
            textureId->w = rifle.AK.w / sacle;
            textureId->h = rifle.AK.h / sacle;
            textureId->textureId = rifle.AK.textureId;
            *name = "AKM";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_M416_Wrapper_C") == 0) {
            textureId->w = rifle.M416.w / sacle;
            textureId->h = rifle.M416.h / sacle;
            textureId->textureId = rifle.M416.textureId;
            *name = "M416";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_M16A4_Wrapper_C") == 0) {
            textureId->w = rifle.M16A4.w / sacle;
            textureId->h = rifle.M16A4.h / sacle;
            textureId->textureId = rifle.M16A4.textureId;
            *name = "M16A4";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_SCAR_Wrapper_C") == 0) {
            textureId->w = rifle.SCAR.w / sacle;
            textureId->h = rifle.SCAR.h / sacle;
            textureId->textureId = rifle.SCAR.textureId;
            *name = "SCAR";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_QBZ_Wrapper_C") == 0) {
            textureId->w = rifle.QBZ.w / sacle;
            textureId->h = rifle.QBZ.h / sacle;
            textureId->textureId = rifle.QBZ.textureId;
            *name = "QBZ";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_G36_Wrapper_C") == 0) {
            textureId->w = rifle.G36C.w / sacle;
            textureId->h = rifle.G36C.h / sacle;
            textureId->textureId = rifle.G36C.textureId;
            *name = "G36C";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_M762_Wrapper_C") == 0) {
            textureId->w = rifle.M762.w / sacle;
            textureId->h = rifle.M762.h / sacle;
            textureId->textureId = rifle.M762.textureId;
            *name = "M762";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_Groza_Wrapper_C") == 0) {
            textureId->w = rifle.GROZA.w / sacle;
            textureId->h = rifle.GROZA.h / sacle;
            textureId->textureId = rifle.GROZA.textureId;
            *name = "Groza";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Rifle_AUG_Wrapper_C") == 0) {
            textureId->w = rifle.AUG.w / sacle;
            textureId->h = rifle.AUG.h / sacle;
            textureId->textureId = rifle.AUG.textureId;
            *name = "AUG";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Other_Dp-28_Wrapper_C") == 0) {
            textureId->w = rifle.DP28.w / sacle;
            textureId->h = rifle.DP28.h / sacle;
            textureId->textureId = rifle.DP28.textureId;
            *name = "Dp-28";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Other_M249_Wrapper_C") == 0) {
            textureId->w = rifle.M249.w / sacle;
            textureId->h = rifle.M249.h / sacle;
            textureId->textureId = rifle.M249.textureId;
            *name = "M249";
            *color = rifle_color;
            return true;
        }
        if (strcmp(gname, "BP_Sniper_QBU_Wrapper_C") == 0) {
            textureId->w = rifle.QBU.w / sacle;
            textureId->h = rifle.QBU.h / sacle;
            textureId->textureId = rifle.QBU.textureId;
            *name = "QBU";
            *color = rifle_color;
            return true;
        }
    }
    //显示冲锋枪
    if (showSubmachine) {
        textureId->x = 40;
        float sacle = 7.5f;
        if (strcmp(gname, "BP_MachineGun_UMP9_Wrapper_C") == 0) {
            textureId->w = charge.UMP45.w / sacle;
            textureId->h = charge.UMP45.h / sacle;
            textureId->textureId = charge.UMP45.textureId;
            *name = "UMP9";
            *color = submachine_color;
            return true;
        }
        if (strcmp(gname, "BP_MachineGun_TommyGun_Wrapper_C") == 0) {
            textureId->w = charge.TANGMUXUN.w / sacle;
            textureId->h = charge.TANGMUXUN.h / sacle;
            textureId->textureId = charge.TANGMUXUN.textureId;
            *name = "汤姆逊冲锋枪";
            *color = submachine_color;
            return true;
        }
        if (strcmp(gname, "BP_MachineGun_PP19_Wrapper_C") == 0) {
            textureId->w = charge.YENIU.w / sacle;
            textureId->h = charge.YENIU.h / sacle;
            textureId->textureId = charge.YENIU.textureId;
            *name = "PP19";
            *color = submachine_color;
            return true;
        }
        if (strcmp(gname, "BP_MachineGun_Uzi_Wrapper_C") == 0) {
            textureId->w = charge.UZI.w / sacle;
            textureId->h = charge.UZI.h / sacle;
            textureId->textureId = charge.UZI.textureId;
            *name = "Uzi";
            *color = submachine_color;
            return true;
        }
        if (strcmp(gname, "BP_MachineGun_Vector_Wrapper_C") == 0) {
            textureId->w = charge.VECTOR.w / sacle;
            textureId->h = charge.VECTOR.h / sacle;
            textureId->textureId = charge.VECTOR.textureId;
            *name = "Vector";
            *color = submachine_color;
            return true;
        }
    }
    //显示狙击枪
    if (showSniper) {
        textureId->x = 40;
        float sacle = 7.5f;
        if (strcmp(gname, "BP_Sniper_Kar98k_Wrapper_C") == 0) {
            textureId->w = Sniper.K98.w / sacle;
            textureId->h = Sniper.K98.h / sacle;
            textureId->textureId = Sniper.K98.textureId;
            *name = "Kar98k";
            *color = snipe_color;
            return true;
        }
        if (strstr(gname, "Mosin") != nullptr) {
            textureId->w = Sniper.K98.w / sacle;
            textureId->h = Sniper.K98.h / sacle;
            textureId->textureId = Sniper.K98.textureId;
            *name = "莫辛纳甘狙击枪";
            *color = snipe_color;
            return true;
        }

        if (strcmp(gname, "BP_Sniper_Mini14_Wrapper_C") == 0) {
            textureId->w = Sniper.MINI14.w / sacle;
            textureId->h = Sniper.MINI14.h / sacle;
            textureId->textureId = Sniper.MINI14.textureId;
            *name = "Mini14";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_Sniper_SKS_Wrapper_C") == 0) {
            textureId->w = Sniper.SKS.w / sacle;
            textureId->h = Sniper.SKS.h / sacle;
            textureId->textureId = Sniper.SKS.textureId;
            *name = "SKS";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_Sniper_M24_Wrapper_C") == 0) {
            textureId->w = Sniper.M24.w / sacle;
            textureId->h = Sniper.M24.h / sacle;
            textureId->textureId = Sniper.M24.textureId;
            *name = "M24";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_rifle_Mk47_Wrapper_C") == 0) {
            textureId->w = Sniper.MK47.w / sacle;
            textureId->h = Sniper.MK47.h / sacle;
            textureId->textureId = Sniper.MK47.textureId;
            *name = "Mk47";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_WEP_Mk14_Pickup_C") == 0) {
            textureId->w = Sniper.MK14.w / sacle;
            textureId->h = Sniper.MK14.h / sacle;
            textureId->textureId = Sniper.MK14.textureId;
            *name = "MK14";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_Sniper_AWM_Wrapper_C") == 0) {
            textureId->w = Sniper.AWM.w / sacle;
            textureId->h = Sniper.AWM.h / sacle;
            textureId->textureId = Sniper.AWM.textureId;
            *name = "AWM";
            *color = snipe_color;
            return true;
        }
        if (strcmp(gname, "BP_Sniper_SLR_Wrapper_C") == 0) {
            textureId->w = Sniper.SLR.w / sacle;
            textureId->h = Sniper.SLR.h / sacle;
            textureId->textureId = Sniper.SLR.textureId;
            *name = "SLR";
            *color = snipe_color;
            return true;
        }
    }
    //倍镜
    if (showMirror) {
        textureId->x = 10;
        float sacle = 7.0f;
        if (strstr(gname, "MZJ_8X") != nullptr) {
            textureId->w = part.babei.w / sacle;
            textureId->h = part.babei.h / sacle;
            textureId->textureId = part.babei.textureId;
            *name = "8倍镜";
            *color = mirror_color;
            return true;
        }
        if (strstr(gname, "MZJ_6X") != nullptr) {
            textureId->w = part.liubei.w / sacle;
            textureId->h = part.liubei.h / sacle;
            textureId->textureId = part.liubei.textureId;
            *name = "6倍镜";
            *color = mirror_color;
            return true;
        }
        if (strstr(gname, "MZJ_4X") != nullptr) {
            textureId->w = part.sibei.w / sacle;
            textureId->h = part.sibei.h / sacle;
            textureId->textureId = part.sibei.textureId;
            *name = "4倍镜";
            *color = mirror_color;
            return true;
        }
        if (strstr(gname, "MZJ_3X") != nullptr) {
            textureId->w = part.sanbei.w / sacle;
            textureId->h = part.sanbei.h / sacle;
            textureId->textureId = part.sanbei.textureId;
            *name = "3倍镜";
            *color = mirror_color;
            return true;
        }
        if (strstr(gname, "MZJ_2X") != nullptr) {
            textureId->w = part.erbei.w / sacle;
            textureId->h = part.erbei.h / sacle;
            textureId->textureId = part.erbei.textureId;
            *name = "2倍镜";
            *color = mirror_color;
            return true;
        }
        if (strstr(gname, "MZJ_HD") != nullptr) {
            textureId->w = part.hongdian.w / sacle;
            textureId->h = part.hongdian.h / sacle;
            textureId->textureId = part.hongdian.textureId;
            *name = "红点瞄准镜";
            *color = otherparts_color;
            return true;
        }
        if (strstr(gname, "MZJ_QX") != nullptr) {
            textureId->w = part.quanxi.w / sacle;
            textureId->h = part.quanxi.h / sacle;
            textureId->textureId = part.quanxi.textureId;
            *name = "全息瞄准镜";
            *color = otherparts_color;
            return true;
        }
    }
    //配件
    if (showExpansion) {
        textureId->x = 10;
        float sacle = 7.5f;
        if (strcmp(gname, "BP_DJ_Large_EQ_Pickup_C") == 0) {
            textureId->w = parts.bq_kuaikuo.w / sacle;
            textureId->h = parts.bq_kuaikuo.h / sacle;
            textureId->textureId = parts.bq_kuaikuo.textureId;
            *name = "步枪快扩";
            *color = expansion_color;
            return true;
        }
        if (strcmp(gname, "BP_DJ_Large_E_Pickup_C") == 0) {
            textureId->w = parts.bq_kuorong.w / sacle;
            textureId->h = parts.bq_kuorong.h / sacle;
            textureId->textureId = parts.bq_kuorong.textureId;
            *name = "步枪扩容";
            *color = expansion_color;
            return true;
        }
        if (strcmp(gname, "BP_DJ_Sniper_EQ_Pickup_C") == 0) {
            textureId->w = parts.jj_kuaikuo.w / sacle;
            textureId->h = parts.jj_kuaikuo.h / sacle;
            textureId->textureId = parts.jj_kuaikuo.textureId;
            *name = "狙击枪快扩";
            *color = mirror_color;
            return true;
        }
        if (strcmp(gname, "BP_DJ_Sniper_E_Pickup_C") == 0) {
            textureId->w = parts.jj_kuorong.w / sacle;
            textureId->h = parts.jj_kuorong.h / sacle;
            textureId->textureId = parts.jj_kuorong.textureId;
            *name = "狙击枪扩容";
            *color = mirror_color;
            return true;
        }
    }
    //其他配件
    if (showOtherParts) {
        textureId->x = 10;
        float sacle = 7.5f;
        if (strcmp(gname, "BP_QK_Large_Suppressor_Pickup_C") == 0) {
            textureId->w = parts.bq_xiaoyinqi.w / sacle;
            textureId->h = parts.bq_xiaoyinqi.h / sacle;
            textureId->textureId = parts.bq_xiaoyinqi.textureId;
            *name = "步枪消音";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_QK_Sniper_Suppressor_Pickup_C") == 0) {
            textureId->w = parts.jj_xiaoyanqi.w / sacle;
            textureId->h = parts.jj_xiaoyinqi.h / sacle;
            textureId->textureId = parts.jj_xiaoyinqi.textureId;
            *name = "狙击枪消音";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_QT_Sniper_Pickup_C") == 0) {
            textureId->w = parts.jj_tuosaiban.w / sacle;
            textureId->h = parts.jj_tuosaiban.h / sacle;
            textureId->textureId = parts.jj_tuosaiban.textureId;
            *name = "狙击枪托腮板";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_ZDD_Sniper_Pickup_C") == 0) {
            textureId->w = parts.sd_zidandai.w / sacle;
            textureId->h = parts.sd_zidandai.h / sacle;
            textureId->textureId = parts.sd_zidandai.textureId;
            *name = "狙击枪子弹袋";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_QK_Large_Compensator_Pickup_C") == 0) {
            textureId->w = parts.bq_buchang.w / sacle;
            textureId->h = parts.bq_buchang.h / sacle;
            textureId->textureId = parts.bq_buchang.textureId;
            *name = "步枪枪口补偿";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_QK_Sniper_Compensator_Pickup_C") == 0) {
            textureId->w = parts.jj_buchang.w / sacle;
            textureId->h = parts.jj_buchang.h / sacle;
            textureId->textureId = parts.jj_buchang.textureId;
            *name = "狙击枪补偿器";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_WB_Vertical_Pickup_C") == 0) {
            textureId->w = parts.chuizhiwoba.w / sacle;
            textureId->h = parts.chuizhiwoba.h / sacle;
            textureId->textureId = parts.chuizhiwoba.textureId;
            *name = "步槍垂直握把";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_QT_A_Pickup_C") == 0) {
            textureId->w = parts.m416_qiangtuo.w / sacle;
            textureId->h = parts.m416_qiangtuo.h / sacle;
            textureId->textureId = parts.m416_qiangtuo.textureId;
            *name = "步枪枪托";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_WB_Angled_Pickup_C") == 0) {
            textureId->w = parts.zhijiaowoba.w / sacle;
            textureId->h = parts.zhijiaowoba.h / sacle;
            textureId->textureId = parts.zhijiaowoba.textureId;
            *name = "步枪直角握把";
            *color = otherparts_color;
            return true;
        }
        if (strcmp(gname, "BP_WB_ThumbGrip_Pickup_C") == 0) {
            textureId->w = parts.muzhiwoba.w / sacle;
            textureId->h = parts.muzhiwoba.h / sacle;
            textureId->textureId = parts.muzhiwoba.textureId;
            *name = "步枪拇指握把";
            *color = otherparts_color;
            return true;
        }
        if (strstr(gname, "Ghillie") != nullptr) {
            textureId->w = Helmet.jilifu.w / sacle;
            textureId->h = Helmet.jilifu.h / sacle;
            textureId->textureId = Helmet.jilifu.textureId;
            *name = "吉利服";
            *color = otherparts_color;
            return true;
        }

        /*
           if (isContain(s, "CheekPad"))
               return "CheekPad";

           if ((isContain(s, "AirDropPlane")))
               return "放置飞机";
           if ((isContain(s, "BP_AirDropPlane_C")))
               return "电动滑翔机";
               */
    }
    //药品显示
    if (showDrug) {
        textureId->x = 10;
        float sacle = 7.5f;
        if (strcmp(gname, "Injection_Pickup_C") == 0) {
            textureId->w = drug.shenshangxiansu.w / sacle;
            textureId->h = drug.shenshangxiansu.h / sacle;
            textureId->textureId = drug.shenshangxiansu.textureId;
            *name = "肾上腺素";
            *color = drug_color;
            return true;
        }
        if (strcmp(gname, "Firstaid_Pickup_C") == 0) {
            textureId->w = drug.jijiubao.w / sacle;
            textureId->h = drug.jijiubao.h / sacle;
            textureId->textureId = drug.jijiubao.textureId;
            *name = "急救包";
            *color = drug_color;
            return true;
        }
        if (strcmp(gname, "FirstAidbox_Pickup_C") == 0) {
            textureId->w = drug.yiliaoxiang.w / sacle;
            textureId->h = drug.yiliaoxiang.h / sacle;
            textureId->textureId = drug.yiliaoxiang.textureId;
            *name = "医疗箱";
            *color = drug_color;
            return true;
        }
        if (strcmp(gname, "Pills_Pickup_C") == 0) {
            textureId->w = drug.zhitengyao.w / sacle;
            textureId->h = drug.zhitengyao.h / sacle;
            textureId->textureId = drug.zhitengyao.textureId;
            *name = "止痛药";
            *color = drug_color;
            return true;
        }
        if (strcmp(gname, "Drink_Pickup_C") == 0) {
            textureId->w = drug.yinliao.w / sacle;
            textureId->h = drug.yinliao.h / sacle;
            textureId->textureId = drug.yinliao.textureId;
            *name = "能量饮料";
            *color = drug_color;
            return true;
        }
        if (strcmp(gname, "Bandage_Pickup_C") == 0) {
            textureId->w = drug.bengdai.w / sacle;
            textureId->h = drug.bengdai.h / sacle;
            textureId->textureId = drug.bengdai.textureId;
            *name = "止血绷带";
            *color = drug_color;
            return true;
        }
    }
    //甲和头盔
    if (showArmor) {
        textureId->x = 10;
        float sacle = 6.5f;
        if (strstr(gname, "Helmet_Lv3") != nullptr) {
            textureId->w = Helmet.sanjitou.w / sacle;
            textureId->h = Helmet.sanjitou.h / sacle;
            textureId->textureId = Helmet.sanjitou.textureId;
            *name = "三级头";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Armor_Lv3") != nullptr) {
            textureId->w = Helmet.sanjijia.w / sacle;
            textureId->h = Helmet.sanjijia.h / sacle;
            textureId->textureId = Helmet.sanjijia.textureId;
            *name = "三级甲";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Bag_Lv3") != nullptr) {
            textureId->w = Helmet.sanjibao.w / sacle;
            textureId->h = Helmet.sanjibao.h / sacle;
            textureId->textureId = Helmet.sanjibao.textureId;
            *name = "三级包";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Helmet_Lv2") != nullptr) {
            textureId->w = Helmet.erjitou.w / sacle;
            textureId->h = Helmet.erjitou.h / sacle;
            textureId->textureId = Helmet.erjitou.textureId;
            *name = "二级头";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Armor_Lv2") != nullptr) {
            textureId->w = Helmet.erjijia.w / sacle;
            textureId->h = Helmet.erjijia.h / sacle;
            textureId->textureId = Helmet.erjijia.textureId;
            *name = "二级甲";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Bag_Lv2") != nullptr) {
            textureId->w = Helmet.erjibao.w / sacle;
            textureId->h = Helmet.erjibao.h / sacle;
            textureId->textureId = Helmet.erjibao.textureId;
            *name = "二级包";
            *color = armor_color;
            return true;
        }

        if (strstr(gname, "Helmet_Lv1") != nullptr) {
            textureId->w = Helmet.yijitou.w / sacle;
            textureId->h = Helmet.yijitou.h / sacle;
            textureId->textureId = Helmet.yijitou.textureId;
            *name = "一级头";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Armor_Lv1") != nullptr) {
            textureId->w = Helmet.yijijia.w / sacle;
            textureId->h = Helmet.yijijia.h / sacle;
            textureId->textureId = Helmet.yijijia.textureId;
            *name = "一级甲";
            *color = armor_color;
            return true;
        }
        if (strstr(gname, "Bag_Lv1") != nullptr) {
            textureId->w = Helmet.yijibao.w / sacle;
            textureId->h = Helmet.yijibao.h / sacle;
            textureId->textureId = Helmet.yijibao.textureId;
            *name = "一级包";
            *color = armor_color;
            return true;
        }
    }

    return false;
}

//获取载具
bool GetVehicleInfo(char *gname, char **name, TextureInfo *textureId, ImVec4 *color) {

    if (strcmp(gname, "VH_BRDM_C") == 0) {
        *textureId = vehicle.zhuangjiache;
        *name = "装甲车";
        *color = vehicle_color;
        return true;
    }
    if (strcmp(gname, "VH_Scooter_C") == 0) {
        *textureId = vehicle.mianyang;
        *name = "电瓶车";
        *color = vehicle_color;
        return true;
    }
    if (strcmp(gname, "VH_Motorcycle_C") == 0 || strcmp(gname, "VH_Motorcycle_1_C") == 0) {
        *textureId = vehicle.motuo;
        *name = "摩托車";
        *color = vehicle_color;
        return true;
    }
    if (strcmp(gname, "VH_MotorcycleCart_1_C") == 0 || strcmp(gname, "VH_MotorcycleCart_C") == 0) {
        *textureId = vehicle.sanlunmotuo;
        *name = "三轮车";
        *color = vehicle_color;
        return true;
    }
    if (strcmp(gname, "VH_Snowmobile_C") == 0) {
        *textureId = vehicle.xuedimotuo;
        *name = "雪地摩托";
        *color = vehicle_color;
        return true;
    }
    if (strcmp(gname, "BP_VH_Tuk_C") == 0 || strcmp(gname, "BP_VH_Tuk_1_C") == 0) {
        *textureId = vehicle.sanlunmotuo;
        *name = "三轮摩托";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "Buggy") != nullptr) {
        *textureId = vehicle.bengbeng;
        *name = "蹦蹦";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "Dacia") != nullptr) {
        *textureId = vehicle.jiaoche;
        *name = "小轿车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "UAZ") != nullptr) {
        *textureId = vehicle.jeep;
        *name = "吉普车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "_PickUp") != nullptr) {
        *textureId = vehicle.wupengpika;
        *name = "皮卡车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "Rony") != nullptr) {
        *textureId = vehicle.luonipika;
        *name = "皮卡车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "Mirado") != nullptr) {
        *textureId = vehicle.paoche;
        *name = "跑车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "MiniBus") != nullptr) {
        *textureId = vehicle.daba;
        *name = "宝宝巴士";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "PG117") != nullptr) {
        *textureId = vehicle.kuaiting;
        *name = "快艇";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "AquaRail") != nullptr) {
        *textureId = vehicle.motuoting;
        *name = "摩托艇";
        *color = vehicle_color;
        return true;
    }

    if (strstr(gname, "AquaRail") != nullptr) {
        *textureId = vehicle.motuoting;
        *name = "摩托艇";
        *color = vehicle_color;
        return true;
    }
    if ((strstr(gname, "AirDropPlane")) != nullptr) {
        *textureId = vehicle.huaxiaji;
        *name = "放置飞机";
        *color = vehicle_color;
        return true;
    }
    if ((strstr(gname, "BP_AirDropPlane_C")) != nullptr) {
        *textureId = vehicle.huaxiaji;
        *name = "电动滑翔机";
        *color = vehicle_color;
        return true;
    }


    return false;
}

bool GetWeaponsInfo(int id, char **name, TextureInfo *textureInfo) {
    /*
    if (id == "CrossBow") {
        *textureInfo = hand.十字弩;
        *name = "十字弩";
        return true;
    }*/
    if (id == 101005) {
        *textureInfo = hand.Grozy;
        *name = "狗砸";
        return true;
    }
    if (id == 103004) {
        *textureInfo = hand.SKS;
        *name = "SKS";
        return true;
    }
    if (id == 105001) {
        *textureInfo = hand.M249;
        *name = "M249";
        return true;
    }
    if (id == 105002) {
        *textureInfo = hand.DP28;
        *name = "DP28";
        return true;
    }
    if (id == 103001) {
        *textureInfo = hand.K98;
        *name = "Kar98k";
        return true;
    }
    if (id == 103903) {
        *textureInfo = hand.K98;
        *name = "莫辛甘纳";
        return true;
    }

    if (id == 106003) {
        *textureInfo = hand.P18C;
        *name = "R1895";
        return true;
    }
    if (id == 106005) {
        *textureInfo = hand.P18C;
        *name = "R45";
        return true;
    }
    if (id == 101001) {
        *textureInfo = hand.AKM;
        *name = "AKM";
        return true;
    }
    if (id == 101006) {
        *textureInfo = hand.AUG;
        *name = "AUG";
        return true;
    }
    if (id == 103006) {
        *textureInfo = hand.Mini14;
        *name = "Mini14";
        return true;
    }
    if (id == 101002) {
        *textureInfo = hand.M416A4;
        *name = "M16A4";
        return true;
    }
    if (id == 101003) {
        *textureInfo = hand.Scar;
        *name = "SCAR";
        return true;
    }
    if (id == 103008) {
        *textureInfo = hand.WIN94;
        *name = "WIN94";
        return true;
    }
    if (id == 102001) {
        *textureInfo = hand.UZI;
        *name = "UZI";
        return true;
    }
    if (id == 102004) {
        *textureInfo = hand.汤姆逊;
        *name = "汤姆逊";
        return true;
    }
    if (id == 102003) {
        *textureInfo = hand.Vector;
        *name = "Vector";
        return true;
    }
    if (id == 102002) {
        *textureInfo = hand.UMP9;
        *name = "UMP9";
        return true;
    }
    if (id == 103005) {
        *textureInfo = hand.VSS;
        *name = "VSS";
        return true;
    }
    if (id == 101004) {
        *textureInfo = hand.M416;
        *name = "M416";
        return true;
    }
    if (id == 103007) {
        *textureInfo = hand.MK14;
        *name = "MK14";
        return true;
    }
    if (id == 103009) {
        *textureInfo = hand.SLR;
        *name = "SLR";
        return true;
    }
    if (id == 101009) {
        *textureInfo = hand.MK47;
        *name = "Mk47";
        return true;
    }
    if (id == 101008) {
        *textureInfo = hand.M762;
        *name = "M762";
        return true;
    }
    if (id == 101007) {
        *textureInfo = hand.QBZ;
        *name = "QBZ";
        return true;
    }
    if (id == 103010) {
        *textureInfo = hand.QBU;
        *name = "QBU";
        return true;
    }
    if (id == 104002) {
        *textureInfo = hand.S1896;
        *name = "S1897";
        return true;
    }
    if (id == 104003) {
        *textureInfo = hand.S12K;
        *name = "S12K";
        return true;
    }
    if (id == 104001) {
        *textureInfo = hand.S686;
        *name = "S686";
        return true;
    }
    /*
    if (id == "FlareGun") {
        *textureInfo = hand.短管散弹;
        *name = "短管散彈";
        return true;
    }
    if (id == "Shoulei") {
        *textureInfo = hand.手雷;
        *name = "手雷";
        return true;
    }
    if (id == "Burn") {
        *textureInfo = hand.烟雾弹;
        *name = "烟霧彈";
        return true;
    }
    if (id ==) {
        *textureInfo = hand.拳头;
        *name = "拳頭";
        return true;
    }*/
    if (id == 103003) {
        *textureInfo = hand.AWM;
        *name = "AWM";
        return true;
    }
    if (id == 103002) {
        *textureInfo = hand.M24;
        *name = "M24";
        return true;
    }
    /*
    if (id == "UMP9") {
        *textureInfo = hand.UMP9;
        *name = "UMP9";
        return true;
    }*/
    textureInfo->w = hand.拳头.w;
    textureInfo->h = hand.拳头.h;
    textureInfo->y = 20;
    textureInfo->textureId = hand.拳头.textureId;
    *name = "拳头";
    return true;
}