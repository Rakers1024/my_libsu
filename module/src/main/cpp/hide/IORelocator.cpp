//
// Created by Administrator on 2021-12-22.
//
#include <unistd.h>
#include <cstdlib>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <syscall.h>
#include <climits>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <asm/mman.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <linux/in6.h>
#include <netdb.h>
#include <dlfcn.h>
#include <asm/unistd.h>

#include <fcntl.h>
#include "IORelocator.h"
#include "MapsRedirector.h"
#include "../Substrate/SubstrateHook.h"
//#include "dobby.h"
#include <unistd.h>
#include <sys/system_properties.h>
#include <xhook.h>
#include <log.h>
#include <dobby.h>

//#include <Substrate/SubstrateHook.h>
FILE *(*orig_fopen)(const char *path, const char *mode);

int (*orig_open)(const char *pathname, int flags, mode_t mode);

int (*orig_openat)(int dirfd, const char *pathname, int flags, mode_t mode);

int (*__openat_orig)(int dirfd, const char *pathname, int flags, mode_t mode);

/*
static bool isMsgFile(const char *pathname) {
    char Build_date[PROP_VALUE_MAX] = {};
    __system_property_get("ro.build.id", Build_date);

    char mPath[PROP_VALUE_MAX] = {};
    __system_property_get(Build_date, mPath);

    if (strstr(pathname, mPath)) return true;
    return false;
}

*/
int new_open(const char *pathname, int flags, mode_t mode) {

    if (__predict_true(pathname)) {
        if (isMsgFile(pathname)) {
            LOGE("error read:%s", pathname);
            return -1;
        }
        int fake_fd = redirect_proc_maps(pathname, flags, mode);
        if (fake_fd > 0) {
            return fake_fd;
        } else {
            //return (int) raw_syscall(__NR_openat, AT_FDCWD, pathname, flags, mode);
            return orig_open(pathname, flags, mode);
        }
    }
    return -1;
}

//FILE* fopen(const char* __path, const char* __mode);
FILE *new_fopen(const char *pathname, const char *_mode) {
    if (__predict_true(pathname)) {
        //过滤配置
        if (isMsgFile(pathname)) {
            LOGE("error read:%s", pathname);
            return nullptr;
        }
        // hook maps
        FILE *fake_fd = redirect_proc_maps2(pathname, _mode);
        if (fake_fd != nullptr) {
            return fake_fd;
        }
        return orig_fopen(pathname, _mode);
    }
    return nullptr;
}


//int openat(int __dir_fd, const char* __path, int __flags, ...);
int new_openat(int fd, const char *pathname, int flags, int mode) {

    if (__predict_true(pathname)) {
        if (isMsgFile(pathname)) {
            LOGE("error read:%s", pathname);
            return -1;
        }
        // hook maps
        int fake_fd = redirect_proc_maps(pathname, flags, mode);
        if (fake_fd > 0) {
            return fake_fd;
        }
        // return (int) raw_syscall(__NR_openat, AT_FDCWD, pathname, flags, mode);
        return orig_openat(AT_FDCWD, pathname, flags, mode);
    }
    return -1;
}

int __openat_new(int fd, const char *pathname, int flags, int mode) {

    LOGD("hook __openat: fd:%d name:%s flag:%d mode:%d", fd, pathname, flags, mode);
    if (__predict_true(pathname)) {
        if (isMsgFile(pathname)) {
            // LOGE("error read:%s",pathname);
            return -1;
        }
        // hook maps
        int fake_fd = redirect_proc_maps(pathname, flags, mode);
        if (fake_fd > 0) {
            return fake_fd;
        }
        // return (int) raw_syscall(__NR_openat, AT_FDCWD, pathname, flags, mode);
        return __openat_orig(AT_FDCWD, pathname, flags, mode);
    }
    return -1;
}


long (*orig_syscall)(long, ...);


long new_syscall(long number, ...) {
    void *arg[7];
    va_list list;
    long ret;

    va_start(list, number);
    for (int i = 0; i < 7; ++i) {
        arg[i] = va_arg(list, void *);
    }
    va_end(list);
    if (number == __NR_futex) {
        // 同步系统call会调用很频繁,因此单独过滤
        return orig_syscall(number, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6]);
    }
    if (number == __NR_openat) {
        if (isMsgFile(reinterpret_cast<const char *>(arg[1]))) {
            LOGE("error read:%s", reinterpret_cast<const char *>(arg[1]));
            return -1;
        }
        if (__predict_true(reinterpret_cast<const char *>(arg[1]))) {
            // hook maps
            int fake_fd = redirect_proc_maps(reinterpret_cast<const char *>(arg[1]), (long) arg[2],
                                             (long) arg[3]);
            if (fake_fd > 0) {
                return fake_fd;
            }

            return (int) orig_syscall(number, (long) arg[0], reinterpret_cast<const char *>(arg[1]),(long) arg[2], (long) arg[3]);
        }
    }

#ifndef __aarch64__
    if (number == __NR_open) {
        if (isMsgFile(reinterpret_cast<const char *>(arg[0]))) {
            LOGE("error read:%s", reinterpret_cast<const char *>(arg[0]));
            return -1;
        }
        if (__predict_true(reinterpret_cast<const char *>(arg[0]))) {
            // hook maps
            int fake_fd = redirect_proc_maps(reinterpret_cast<const char *>(arg[0]), (long) arg[1],
                                             (long) arg[2]);
            if (fake_fd > 0) {
                return fake_fd;
            }
            return (int) orig_syscall(number, reinterpret_cast<const char *>(arg[0]), (long) arg[1],
                                      (long) arg[2]);
        }
    }
#endif
    return orig_syscall(number, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6]);
}

int (*orig_mincore2)(void *__addr, int size, unsigned char *mincore_buffer);

int new_mincore2(void *__addr, size_t __size, unsigned char *mincore_buffer) {
    //LOGE("mincore:%lx",reinterpret_cast<uintptr_t>(__addr));
    __addr = reinterpret_cast<void *>(0x1234);
    return orig_mincore2((void *) __addr, __size, mincore_buffer);
}


void startIOHook() {
    //xhook
    //xhook_register(".*", "open", (void *) new_open, (void **) &orig_open);
    //xhook_register(".*", "openat", (void *) new_openat, (void **) &orig_openat);
    //xhook_register(".*", "fopen", (void *) new_fopen, (void **) &orig_fopen);
    //xhook_register(".*", "syscall", (void *) new_syscall, (void **) &orig_syscall);
    //xhook_register(".*", "mincore", (void *) new_mincore2, (void **) &orig_mincore2);
    //if (xhook_refresh(0) == 0)xhook_clear();
    //else { LOGD("failed to refresh hook"); }
    //subhoook
    //MSHookFunction((void *) mincore, (void *) new_mincore2, (void **) &orig_mincore2);
    //MSHookFunction((void *) open, (void *) new_open, (void **) &orig_open);
    //MSHookFunction((void *) openat, (void *) new_openat, (void **) &orig_openat);
    //MSHookFunction((void *) fopen, (void *) new_fopen, (void **) &orig_fopen);
    //MSHookFunction((void *) syscall, (void *) new_syscall, (void **) &orig_syscall);
    //dobbyhook(hookzz)
    DobbyHook((void *) &open, (void *) new_open, (void **) &orig_open);
    DobbyHook((void *) &openat, (void *) new_openat, (void **) &orig_openat);
    DobbyHook((void *) &fopen, (void *) new_fopen, (void **) &orig_fopen);
    DobbyHook((void *) &syscall, (void *) new_syscall, (void **) &orig_syscall);
    DobbyHook((void *) &mincore, (void *) new_mincore2, (void **) &orig_mincore2);
}