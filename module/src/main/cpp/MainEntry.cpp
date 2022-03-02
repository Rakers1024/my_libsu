#include <jni.h>
#include <sys/types.h>
#include <riru.h>
#include <malloc.h>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fcntl.h>
#include <log.h>
#include "imgui_draw_main.h"
#include "zygisk.hpp"

static bool sHookEnable = false;
static char *sAppDataDir = nullptr;

static char *jstringToC(JNIEnv * env, jstring jstr){
    char *ret = NULL;
    if (jstr) {
        const char* str = env->GetStringUTFChars(jstr, NULL);
        if (str != NULL) {
            int len = strlen(str);
            ret = (char*) malloc((len + 1) * sizeof(char));
            if (ret != NULL){
                memset(ret, 0, len + 1);
                memcpy(ret, str, len);
            }
            env->ReleaseStringUTFChars(jstr, str);
        }
    }
    return ret;
}


static bool equals(const char *str1, const char *str2) {
    if (str1 == nullptr && str2 == nullptr) {
        return true;
    } else {
        if (str1 != nullptr && str2 != nullptr) {
            return strcmp(str1, str2) == 0;
        } else {
            return false;
        }
    }
}

static void pre(JNIEnv *env, jstring *appDataDir, jstring *niceName) {
    char *cAppDataDir = jstringToC(env, *appDataDir);
    if (cAppDataDir == nullptr) {
        return;
    }
    sAppDataDir = strdup(cAppDataDir);
    free(cAppDataDir);
    if (sAppDataDir == nullptr) {
        return;
    }
    char *cNiceName = jstringToC(env, *niceName);
    sHookEnable = equals(cNiceName, "com.tencent.tmgp.pubgmhd");
    if (cNiceName) {
        free(cNiceName);
    }
}


static void post(JNIEnv *env) {
    if (sHookEnable) {
        LOGD("riru inited");
        pthread_t t;
        pthread_create(&t, nullptr, main_thread, nullptr);
    }
}


static int shouldSkipUid(int uid) {
    return false;
}

static void forkAndSpecializePre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jintArray *fdsToClose, jintArray *fdsToIgnore, jboolean *is_child_zygote,
        jstring *instructionSet, jstring *appDataDir, jboolean *isTopApp, jobjectArray *pkgDataInfoList,
        jobjectArray *whitelistedDataInfoList, jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    pre(env, appDataDir, niceName);

}

static void forkAndSpecializePost(JNIEnv *env, jclass clazz, jint res) {
    if (res == 0) {
        post(env);
    }
}

static void specializeAppProcessPre(
        JNIEnv *env, jclass clazz, jint *uid, jint *gid, jintArray *gids, jint *runtimeFlags,
        jobjectArray *rlimits, jint *mountExternal, jstring *seInfo, jstring *niceName,
        jboolean *startChildZygote, jstring *instructionSet, jstring *appDataDir,
        jboolean *isTopApp, jobjectArray *pkgDataInfoList, jobjectArray *whitelistedDataInfoList,
        jboolean *bindMountAppDataDirs, jboolean *bindMountAppStorageDirs) {
    pre(env, appDataDir, niceName);
}

static void specializeAppProcessPost(JNIEnv *env, jclass clazz) {
    post(env);
    //模块hook需要设置false
    riru_set_unload_allowed(false);
}

static int urandom = -1;

static void CompanionEntry(int i) {
    if (urandom < 0) {
        urandom = open("/dev/urandom", O_RDONLY);
    }
    unsigned r;
    read(urandom, &r, sizeof(r));
    LOGD("example: companion r=[%u]\n", r);
    write(i, &r, sizeof(r));
}

class ZygiskModule : public zygisk::ModuleBase {

    zygisk::Api *api;
    JNIEnv *env;

    void onLoad(zygisk::Api *api_, JNIEnv *env_) override {
        this->api = api_;
        this->env = env_;

    }

    void preAppSpecialize(zygisk::AppSpecializeArgs *args) override {
        const char *process = env->GetStringUTFChars(args->nice_name, nullptr);
        sHookEnable = equals(process, "com.tencent.tmgp.pubgmhd");
        LOGE("Pre加载的应用:%s,%d", process, sHookEnable);
        preSpecialize(process);
        env->ReleaseStringUTFChars(args->nice_name, process);
    }

    void preServerSpecialize(zygisk::ServerSpecializeArgs *args) override {
        preSpecialize("system_server");
    }

    void postAppSpecialize(const zygisk::AppSpecializeArgs *args) override {
        if (sHookEnable) {
            LOGD("zygisk inited");
            pthread_t t;
            pthread_create(&t, nullptr, main_thread, nullptr);
        }
    }

    void preSpecialize(const char *process) {
        unsigned r = 0;
        int fd = api->connectCompanion();
        read(fd, &r, sizeof(r));
        close(fd);
        LOGD("example: process=[%s], r=[%u]\n", process, r);
        if(sHookEnable){
            return;
        }
        api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
    }

};



extern "C" {

int riru_api_version;
const char *riru_magisk_module_path = nullptr;
int *riru_allow_unload = nullptr;

static auto module = RiruVersionedModuleInfo{
        .moduleApiVersion = RIRU_MODULE_API_VERSION,
        .moduleInfo= RiruModuleInfo{
                .supportHide = true,
                .version = RIRU_MODULE_VERSION,
                .versionName = RIRU_MODULE_VERSION_NAME,
                .onModuleLoaded = nullptr,
                .forkAndSpecializePre = forkAndSpecializePre,
                .forkAndSpecializePost = forkAndSpecializePost,
                .forkSystemServerPre = nullptr,
                .forkSystemServerPost = nullptr,
                .specializeAppProcessPre = specializeAppProcessPre,
                .specializeAppProcessPost = specializeAppProcessPost
        }
};

RiruVersionedModuleInfo *init(Riru *riru) {
    auto core_max_api_version = riru->riruApiVersion;
    riru_api_version = core_max_api_version <= RIRU_MODULE_API_VERSION ? core_max_api_version : RIRU_MODULE_API_VERSION;
    module.moduleApiVersion = riru_api_version;
    riru_magisk_module_path = strdup(riru->magiskModulePath);
    if (riru_api_version >= 25) {
        riru_allow_unload = riru->allowUnload;
    }
    return &module;
}
}

REGISTER_ZYGISK_MODULE(ZygiskModule)
REGISTER_ZYGISK_COMPANION(CompanionEntry)