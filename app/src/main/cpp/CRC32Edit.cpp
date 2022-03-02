
//CRC32编辑
//输入
//长度
//偏移:         <0 表示末尾添加,  其他值表示在某位偏移位置覆盖
//自己要的crc值
//填充的4字节值,把这个值覆盖到偏移位置就可以实现你要的crc

#include <jni.h>
#include <cstdio>
#include <malloc.h>
#include <cstring>
#include "strings.h"
#include <string>
#include <bits/fcntl.h>
#include <Structs.h>
#include <asm/fcntl.h>
#include <asm/mman.h>
#include <sys/mman.h>
#include <zconf.h>
#include <fstream>
#include <fcntl.h>
#include "include/log.h"

int CRC32Edit(unsigned char *byt, int bytLen, int lOffset, int lcrc, int &retlCRC) {
#define Limit            0xEDB88320
#define Num0                            0
#define num1                            0x1
#define num2                            0x2
#define Num8                            0x8
#define Num255                          0xFF
#define Num256                          0x100
#define Num16777215                     0xFFFFFF
#define dwPolynomial                    0xEDB88320
#define Num2147483647                   0x7FFFFFFF
#define NumNegative1                    0xFFFFFFFF
#define NumNegative2                    0xFFFFFFFE
#define NumNegative256                  0xFFFFFF00

    int CRCdata[256];

    bytLen--;
    int i, x, crc;
    int crcTable[256];
    for (i = 0; i <= 255; i++) {
        for (crc = i, x = 0; x < 8; x++) {
            if (crc & 1)
                crc = (((crc & 0xFFFFFFFE) / 2) & 0x7FFFFFFF) ^ Limit;
            else
                crc = (crc & 0xFFFFFFFE) / 2;
        }
        crcTable[i] = crc;
        CRCdata[i] = crc;
    }
    if (bytLen < 0)return 0;
    unsigned int crcResult = NumNegative1;
    if (lOffset < 0 || lOffset > bytLen)
        lOffset = bytLen + 1;

    for (i = 0; i <= lOffset - 1; i++) {
        crcResult = (crcResult >> 8 ^ crcTable[((crcResult & 255) ^ byt[i]) & 255]);
    }
    int crcFront = ~crcResult;

    int k = bytLen - lOffset - 3;

    int cr1, j;
    if (k > 0) {
        unsigned char *backbyt;
        backbyt = byt + lOffset + 4;
        cr1 = ~lcrc;
        for (j = bytLen; j > bytLen - k; j--) {
            for (i = 0; i <= 255; i++) {
                if ((cr1 & 0xff000000) == (CRCdata[i] & 0xff000000))
                    break;
            }
            cr1 = cr1 ^ CRCdata[i];
            cr1 = cr1 << 8;
            cr1 = (i ^ byt[j]) | cr1;
        }
    } else
        cr1 = ~lcrc;

    for (j = 0; j <= 3; j++) {
        for (i = 0; i <= 255; i++) {
            if ((cr1 & 0xff000000) == (CRCdata[i] & 0xff000000))
                break;
        }
        cr1 = cr1 ^ CRCdata[i];
        cr1 = cr1 << 8;
        cr1 = i | cr1;
    }
    retlCRC = cr1 ^ crcResult;

    return crcFront;
}


char *jstringTostring(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_kmxs_reader_activity_MainActivity_init(JNIEnv *env, jobject thiz, jstring user,jstring token, jstring token2, jstring init_state,jstring init_data) {
    InitData *initData = (InitData *) malloc(sizeof(InitData));
    // 清空文件
    system("echo '' > /storage/emulated/0/imgui_init/init");
    int mmapFd = open("/storage/emulated/0/imgui_init/init", O_CREAT | O_RDWR, 0660);
    if (mmapFd < 1) { return false; }
    // 向文件写入数据
    char data[1024];
    memset(data, 0, 1024);
    strcpy(data, " ");
    write(mmapFd, data, 1024);
    //创建映射
    initData = (InitData *) mmap(nullptr, 1024, PROT_READ | PROT_WRITE, MAP_SHARED,mmapFd, 0);
    close(mmapFd);
    if (initData == nullptr) { return false; }
    initData->init_data= env->GetStringUTFChars(init_data,nullptr),
    initData->init_state=env->GetStringUTFChars(init_state, nullptr),
    initData->user =env->GetStringUTFChars(user, nullptr);
    initData->sToken = env->GetStringUTFChars(token, nullptr);
    initData->eToken = env->GetStringUTFChars(token2, nullptr);
    return true;
}
extern "C"

JNIEXPORT jint JNICALL
Java_com_kmxs_reader_activity_MainActivity_hashTab(JNIEnv *env, jobject thiz, jstring url) {
    // TODO: implement hashTab()
    // TODO: implement hashTab()
    //转为char*
    char *file_path = jstringTostring(env, url);
    //打开文件
    FILE *fp = fopen(file_path, "r");
    if (fp) {
        //到文件尾部
        fseek(fp, 0L, SEEK_END);
        //读大小
        int size = ftell(fp);
        //回到文件头部
        rewind(fp);
        //初始化char*
        unsigned char *data = (unsigned char *) malloc(size * 2);
        //读内容
        fread(data, size, 1, fp);
        //关闭文件
        fclose(fp);

        //计算CRC32覆盖值
        int crcout;
        CRC32Edit(data, size, -1, 0xD27E4835, crcout);
        //追加4字节
        memcpy(data + size, &crcout, 4);

        //打开文件
        FILE *fp_w = fopen(file_path, "w+");
        //写入内容
        fwrite(data, size + 4, 1, fp_w);
        //关闭文件
        fclose(fp_w);
    }
    return 0;
}


/*

void getModule(char* name,uintptr_t libStar,uintptr_t libEnd){
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/self/maps");
    FILE *fp = fopen(filename, "r-xp");
    while (!feof(fp)) {
        fgets(buffer, sizeof(buffer), fp);
        if (strstr(buffer, name)) {
#if defined(__LP64__)
            sscanf(buffer, "%lx-%lx %*s", &libStar, &libEnd);
#else
            sscanf(buffer, "%x-%x %*s", &libtersafeStar,&libtersafeEnd);
#endif
            break;
        }
    }
    fclose(fp);
}

void setLibETF() {
    uintptr_t libsqlcipherStar = 0;// = getModuleAddr("libtersafe.so", true);
    uintptr_t libsqlcipherEnd = 0;//= getModuleAddr("libtersafe.so", false);
    getModule("sqlcipher",libsqlcipherStar,libsqlcipherEnd);

    LOGD("%lx - %lx", libsqlcipherStar, libsqlcipherEnd);
    int j = 0;
    while (j <= (libsqlcipherEnd - libsqlcipherStar - 4)) {
        char *tmpAddr = reinterpret_cast<char *>(libsqlcipherStar + j);
        if (*(tmpAddr + 0) == 0x7F && *(tmpAddr + 1) == 0x45 &&
            *(tmpAddr + 2) == 0x4C && *(tmpAddr + 3) == 0x46) {

            *(reinterpret_cast<int *>(reinterpret_cast<uintptr_t> (tmpAddr))) = 0x00;
            *(reinterpret_cast<int *>(reinterpret_cast<uintptr_t> (tmpAddr + 1))) = 0x00;
            *(reinterpret_cast<int *>(reinterpret_cast<uintptr_t> (tmpAddr + 2))) = 0x00;
            *(reinterpret_cast<int *>(reinterpret_cast<uintptr_t> (tmpAddr + 3))) = 0x00;

            LOGD("找到特征:%lx", reinterpret_cast<uintptr_t> (tmpAddr));
        }
        j++;
    }
    LOGD("没有找到特征");
}


*/
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    //  setLibETF();
    LOGD("加载完成");


    return JNI_VERSION_1_6;

}