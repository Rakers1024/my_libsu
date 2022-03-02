/* 说明：Android JNI Log打印宏定义文件
 */
#ifndef _JNI_UTILS_
#define _JNI_UTILS_

#include <string>
#include <string.h>
#include <string>

#define IS_DEBUG
#ifdef IS_DEBUG
#else
#define LOGI(LOG_TAG, ...) NULL
#define LOGE(LOG_TAG, ...) NULL
#endif

std::string Jstring2string(JNIEnv *env, jstring jstr);
jstring Str2Jstring(JNIEnv *env, const char *pStr);
void replaceAll(char *str);
std::string getTimestamp(JNIEnv *env);
std::string getNetTimestamp();
#endif