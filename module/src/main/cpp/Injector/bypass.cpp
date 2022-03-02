//
// Created by 高旭 on 2021/6/1.
//
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <android/log.h>
#include <fcntl.h>
#include <jni.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <malloc.h>
#include<android/log.h>
#include <dlfcn.h>
#include <pthread.h>
#include <elf.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <regex.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/uio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <elf.h>
#include <fcntl.h>
#include <elf.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <syscall.h>
#include <fstream>
#include <sys/stat.h>
#include <malloc.h>
#include <sys/mman.h>
#include <unistd.h>
#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <android/log.h>
#include <fcntl.h>
#include <jni.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <malloc.h>
#include<android/log.h>
#include <dlfcn.h>
#include <pthread.h>
#include <elf.h>
#include <unistd.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <regex.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/uio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#include <fcntl.h>
#include <elf.h>
#include <malloc.h>
#include <fcntl.h>
#include <syscall.h>
#include <fstream>
#include <sys/stat.h>
#include <malloc.h>
#include <sys/mman.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/types.h>
#include <linux/types.h>
#include <asm-generic/types.h>
#include <asm/unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
//#include "Dobby/include/dobby.h" hookzz
#include "byopen/byopen.h"
#include "../include/Substrate/CydiaSubstrate.h"

static bool isHooked = false;
/**
 * libbypass就是过检 实现代码在init_array段 当游戏dlopen到内存时libc会主动调用init_array段
 * 为了防止被偷你可以注入一个自己写的so so里验证成功再调用dlopen这个bypass
 * 玩一半概率闪退是我循环去查找svc指令去patch了 担心会动态下发脚本所以循环 但是目前来看暂时没用动态指令 可以不循环
 * 暂时没看到有动态下发的指令 所有没必要循环 改成fun2(NULL);就可以不循环了
 * 函数名为了防止ida能看懂我给inline并且改成funxx了
 */

#define addr_t uintptr_t

#define TAG "inject-bypass"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)

/**
 * 系统调用Shell代码
 */
static int ro_syscallShellCode[] = {-509558771, -382926608, -509579264, -509607935, -509603838,
                                    -392429448,
                                    -285212672, -390266640, 1362099998};
static int *fun5 = NULL;

static int (*fun4)(int num, ...);

__attribute((always_inline)) static int (*oldFun)(void *ptr, long size, unsigned char *result);

__attribute((always_inline)) static int newFun(void *addr, long size, unsigned char *result) {
    result[0] = 0;
    return 0;
}

__attribute((always_inline)) static int
(*oldFun2)(int num, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6,
           void *arg7, void *arg8);


__attribute((always_inline)) static int
newFun2(int num, void *arg1, void *arg2, void *arg3, void *arg4, void *arg5, void *arg6, void *arg7,
        void *arg8) {
    if (num == __NR_mincore) {
        ((unsigned char *) (arg3))[0] = 0;
        return 0;
    }
    return fun4(num, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
}

#define ARM_IS_MOV_R7_IMM(insn) (((insn) & 0xFF00F000) == 0xE3007000)

__attribute((always_inline)) static void fun1(addr_t begin, addr_t end) {
    addr_t start = begin;
    addr_t limit = end - sizeof(int) * 4;
    do {
        int *insn = reinterpret_cast<int *>(int(start));
        //             0xE1A02003                           0xE89C0078               0xEF000000
        if (insn[0] == 0xE1A0C007 && ARM_IS_MOV_R7_IMM(insn[1]) && insn[2] == 0xEF000000) {
            int value = insn[1];
            int num = ((value & 0xF0000) >> 4) | (value & 0x00FFF);
            if (num == __NR_mincore) {
                MSHookFunction((void *) insn, (void *) newFun, (void **) &oldFun);
                LOGD("patch fun");
                isHooked = true;
            }
        }
        if (insn[0] == 0xE1A02003 && insn[1] == 0xE89C0078 && insn[2] == 0xEF000000) {
//            MSHookFunction((void *) insn, (void*) newFun2, (void**) &oldFun2);
//            LOGD("svc %p, my %p", insn, fun5);
//            LOGD("patch svc");
        }
        start += 1;
    } while (start < limit);
}

__attribute((always_inline)) static bool fun3(const char *perm) {
    bool r = false, x = false;
    for (int i = 0; i < 5; ++i) {
        if (perm[i] == 'r') {
            r = true;
        }
        if (perm[i] == 'x') {
            x = true;
        }
    }
    return r && x;
}

__attribute((always_inline)) static void *fun2(void *args) {
    //while (!isHooked)
    while (!isHooked) {
        sleep(1);
        FILE *f;
        if ((f = fopen("/proc/self/maps", "r")) == NULL) {
            continue;
        }
        char buf[PATH_MAX + 100], perm[5], dev[6], mapname[PATH_MAX];
        addr_t begin, end, inode, foo;

        while (!feof(f)) {
            if (fgets(buf, sizeof(buf), f) == 0)
                break;
            mapname[0] = '\0';
            sscanf(buf, "%lx-%lx %4s %lx %5s %ld %s", &begin, &end, perm,
                   &foo, dev, &inode, mapname);
            if (strstr(buf, "libc.so") && fun3(perm)) {
                fun1(begin, end);
            }
        }
        fclose(f);
    }
    return nullptr;
}


extern "C" __attribute__((constructor)) void bypass() {
    fun5 = (int *) malloc(sizeof(int) * 9);
    memset(fun5, 0, sizeof(int) * 9);
    memcpy(fun5, ro_syscallShellCode, sizeof(int) * 9);
    unsigned int page_size = sysconf(_SC_PAGESIZE);
    unsigned int page_start = ((long) fun5) & (~(page_size - 1));
    mprotect((void *) page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
    fun4 = (int (*)(int num, ...)) fun5;
    // fun2(NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, fun2, NULL);
}
