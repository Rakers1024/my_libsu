#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <elf.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/uio.h>
#include <stdio.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <dirent.h>
#include <elf.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "../Injector/byopen//byopen.h"
//#include "../Dobby/include/dobby.h"

#define TAG "inject-injector"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

typedef unsigned int pointerType;


/*
unsigned int sc_old[] = {
	// libname
	0xe59f0030, // ldr     r0, [pc, #48] | addr of "libname" in r0
	0xe3a01000, // mov     r1, #0        | r1 = 0 (flags=0)
	0xe1a0e00f, // mov     lr, pc        | populate lr
	0xe59ff028, // ldr     pc, [pc, #40] | call dlopen()
	0xe59fd01c, // ldr     sp, [pc, #28] | fix sp
	0xe59f0008, // ldr     r0, [pc, #12] | fix r0
	0xe59f1008, // ldr     r1, [pc, #12] | fix r1
	0xe59fe008, // ldr     lr, [pc, #12] | fix lr
	0xe59ff008, // ldr     pc, [pc, #12] | fix pc (continue process)
	0xe1a00000, // nop (mov r0,r0)       | r0
	0xe1a00000, // nop (mov r0,r0)       | r1
	0xe1a00000, // nop (mov r0,r0)       | lr
	0xe1a00000, // nop (mov r0,r0)       | pc
	0xe1a00000, // nop (mov r0,r0)       | sp
	0xe1a00000, // nop (mov r0,r0)       | addr of libname
	0xe1a00000  // nop (mov r0,r0)       | dlopen address
};
*/

unsigned int sc[] = {
        0xe59f0040, //        ldr     r0, [pc, #64]   ; 48 <.text+0x48>
        0xe3a01000, //        mov     r1, #0  ; 0x0
        0xe1a0e00f, //        mov     lr, pc
        0xe59ff038, //        ldr     pc, [pc, #56]   ; 4c <.text+0x4c>
        0xe59fd02c, //        ldr     sp, [pc, #44]   ; 44 <.text+0x44>
        0xe59f0010, //        ldr     r0, [pc, #16]   ; 30 <.text+0x30>
        0xe59f1010, //        ldr     r1, [pc, #16]   ; 34 <.text+0x34>
        0xe59f2010, //        ldr     r2, [pc, #16]   ; 38 <.text+0x38>
        0xe59f3010, //        ldr     r3, [pc, #16]   ; 3c <.text+0x3c>
        0xe59fe010, //        ldr     lr, [pc, #16]   ; 40 <.text+0x40>
        0xe59ff010, //        ldr     pc, [pc, #16]   ; 44 <.text+0x44>
        0xe1a00000, //        nop                     r0
        0xe1a00000, //        nop                     r1
        0xe1a00000, //        nop                     r2
        0xe1a00000, //        nop                     r3
        0xe1a00000, //        nop                     lr
        0xe1a00000, //        nop                     pc
        0xe1a00000, //        nop                     sp
        0xe1a00000, //        nop                     addr of libname
        0xe1a00000, //        nop                     dlopenaddr
};

struct pt_regs2 {
    long uregs[18];
};

#define ARM_cpsr uregs[16]
#define ARM_pc uregs[15]
#define ARM_lr uregs[14]
#define ARM_sp uregs[13]
#define ARM_ip uregs[12]
#define ARM_fp uregs[11]
#define ARM_r10 uregs[10]
#define ARM_r9 uregs[9]
#define ARM_r8 uregs[8]
#define ARM_r7 uregs[7]
#define ARM_r6 uregs[6]
#define ARM_r5 uregs[5]
#define ARM_r4 uregs[4]
#define ARM_r3 uregs[3]
#define ARM_r2 uregs[2]
#define ARM_r1 uregs[1]
#define ARM_r0 uregs[0]
#define ARM_ORIG_r0 uregs[17]

#define CPSR_T_MASK (1u << 5)


int findPid(const char *packageName) {
    int id;
    int pid = -1;
    DIR *dir = NULL;
    FILE *fp = NULL;
    char fileName[32] = "";
    char cmdline[256] = "";

    struct dirent *entry;

    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(fileName, "/proc/%d/cmdline", id);
            fp = fopen(fileName, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0) {
                    pid = id;
                    break;
                }
            }
        }
    }
    closedir(dir);

    return pid;
}

pointerType getModuleBase(int pid, const char *moduleName) {
    bool isStart = true;
    char mapsPath[64] = "";
    sprintf(mapsPath, "/proc/%d/maps", pid);
    int fd = syscall(__NR_open, mapsPath, O_RDONLY);

    pointerType start = 0;
    pointerType end = 0;
    bool firstFind = true;
    char line[PATH_MAX];
    char *p = line, *e;
    size_t n = PATH_MAX - 1;
    ssize_t r;
    while ((r = TEMP_FAILURE_RETRY(read(fd, p, n))) > 0) {
        p[r] = '\0';
        p = line; // search begin at line start

        while ((e = strchr(p, '\n')) != NULL) {
            e[0] = '\0';
            if (strstr(p, moduleName) != NULL) {
                if (isStart) {
                    //find start
                    if (firstFind) {
                        start = strtoul(p, NULL, 16);
                        firstFind = false;
                    }
                } else {
                    //find end
                    end = strtoul(p + 9, NULL, 16);
                }
            }
            p = e + 1;
        }
        if (p == line) { // !any_entry
            goto __break;
        } //if

        const size_t remain = strlen(p);
        if (remain <= (PATH_MAX / 2)) {
            memcpy(line, p, remain * sizeof(p[0]));
        } else {
            memmove(line, p, remain * sizeof(p[0]));
        } //if

        p = line + remain;
        n = PATH_MAX - 1 - remain;
    }

    __break:
    syscall(__NR_close, fd);
    return isStart ? start : end;
}

int writeMem(int pid, pointerType *buf, int count, pointerType addr) {
    for (int i = 0; i < count; i++) {
        ptrace(PTRACE_POKEDATA, pid, (void *) (addr + i * sizeof(pointerType)), buf[i]);
    }
    return 0;
}

pointerType findDlopenBase(int pid) {
    pointerType dlopenBase = 0;
    const char *dlopenName[] = {
            "__dl__Z9do_dlopenPKciPK17android_dlextinfoPKv", //11 = 10
            "__dl__Z9do_dlopenPKciPK17android_dlextinfoPv",
            "__dl__ZL10dlopen_extPKciPK17android_dlextinfoPv",
            "__dl__Z20__android_dlopen_extPKciPK17android_dlextinfoPKv",
            "__dl___loader_android_dlopen_ext",
            "__dl__Z9do_dlopenPKciPK17android_dlextinfo",
            "__dl__Z8__dlopenPKciPKv",
            "__dl___loader_dlopen",
            "__dl_dlopen",
    };
    for (int i = 0; i < 9; i++) {
        void *handle = by_dlopen("linker", BY_RTLD_NOW);
        pointerType mySymbolAddr = (pointerType) (handle, dlopenName[i]);
        int off = mySymbolAddr - getModuleBase(getpid(), "linker");
        pointerType targetBase = getModuleBase(pid, "linker");
        dlopenBase = targetBase + off;
        LOGD("mySymbolAddr=%p, off=%p, targetBase=%p, dlopenBase=%p", mySymbolAddr, off, targetBase,
             dlopenBase);
//		find_name(pid, dlopenName[i], "linker", &dlopenBase);
        if (dlopenBase != 0) {
            break;
        }
    }
    return dlopenBase;
}

int doInject(const char *packageName, const char *libraryPath) {
    int bits = sizeof(pointerType);
    int mPid = getpid();
    //Check Root
    if (getuid() != 0) {
        puts("No Root");
        exit(0);
        return 0;
    }
    //Close SELinux
    system("setenforce 0");
    //argv[0] currentPath
    //argv[1] packageName
    //argv[2] libraryPath
//	char packageName[64] = "com.Google.Inc.SimpleApp";
//	char libraryPath[128] = "/data/local/tmp/libSimpleLibrary.so";
//	if (argc >= 3){
//		sprintf(packageName, "%s", argv[1]);
//		sprintf(libraryPath, "%s", argv[2]);
//	}
    //Find Process
    int targetPid = -1;
    while ((targetPid = findPid(packageName)) == -1) {
        puts("Waiting Process...");
        sleep(1);
    }
    LOGD("Find Process, Pid = %d\n", targetPid);

    //Init Library Path Count
    int pathLength = strlen(libraryPath) + 1;
    int pathCount = pathLength / bits + (pathLength % bits == 0 ? 0 : 1);
    //Init Library Path Memory
    char *arg = (char *) malloc(pathCount * bits);
    memset(arg, 0, pathCount * bits);
    memcpy(arg, libraryPath, pathCount * bits);

    //Init Target Dlopen Base

    pointerType mLinkerBase = 0;
    pointerType targetLinkerBase = 0;

    /*
    //find_libbase(mPid, "linker", &mLinkerBase);
    //find_libbase(targetPid, "linker", &targetLinkerBase);
    mLinkerBase = getModuleBase(mPid, "linker");
    targetLinkerBase = getModuleBase(targetPid, "linker");

    pointerType mDlopenBase = 0;
    pointerType targetDlopenBase = 0;

    mDlopenBase = findDlopenBase(mPid);
    targetDlopenBase = targetLinkerBase + (mDlopenBase - mLinkerBase);
    */

    pointerType targetDlopenBase = findDlopenBase(targetPid);
    LOGD("Target Dlopen Base : %lx\n", targetDlopenBase);

    //Attach
    ptrace(PTRACE_ATTACH, targetPid, 0, 0);
    waitpid(targetPid, NULL, WUNTRACED);
    puts("Attach Process");

    ptrace(PTRACE_SYSCALL, targetPid, NULL, 0);
    waitpid(targetPid, NULL, WUNTRACED);
    //connectToZygote(NULL);
    ptrace(PTRACE_SYSCALL, targetPid, NULL, NULL);
    waitpid(targetPid, NULL, WUNTRACED);

    struct pt_regs2 regs, original_regs;
    ptrace(PTRACE_GETREGS, targetPid, 0, &regs);
    memcpy(&original_regs, &regs, sizeof(regs));
    puts("Save Old Regs");

    sc[11] = regs.ARM_r0;
    sc[12] = regs.ARM_r1;
    sc[13] = regs.ARM_r2;
    sc[14] = regs.ARM_r3;
    sc[15] = regs.ARM_lr;
    sc[16] = regs.ARM_pc;
    sc[17] = regs.ARM_sp;
    pointerType libraryNameBase = regs.ARM_sp - pathCount * bits - sizeof(sc);
    sc[18] = libraryNameBase;
    sc[19] = targetDlopenBase;

    // write library name to stack
    writeMem(targetPid, (pointerType *) arg, pathCount, libraryNameBase);
    free(arg);
    pointerType codeaddr = regs.ARM_sp - sizeof(sc);
    writeMem(targetPid, (pointerType *) &sc, sizeof(sc) / sizeof(long), codeaddr);

    // calc stack pointer
    regs.ARM_sp = regs.ARM_sp - pathCount * bits - sizeof(sc);
    // call dlopen()
    regs.ARM_r0 = libraryNameBase;
    regs.ARM_r1 = RTLD_NOW | RTLD_GLOBAL;
    regs.ARM_r2 = 0;

    pointerType targetLibartBase = 0;
    targetLibartBase = getModuleBase(targetPid, "libart");
    regs.ARM_r3 = targetLibartBase + 0x2000;
    regs.ARM_lr = 0;
    regs.ARM_pc = targetDlopenBase;
    //进行指令集判断
    if (regs.ARM_pc & 1) {
        regs.ARM_pc &= (~1u);
        regs.ARM_cpsr |= CPSR_T_MASK;
    } else {
        regs.ARM_cpsr &= ~CPSR_T_MASK;
    }

    ptrace(PTRACE_SETREGS, targetPid, 0, &regs);
    ptrace(PTRACE_CONT, targetPid, NULL, 0);

    int stat = 0;
    waitpid(targetPid, &stat, WUNTRACED);
    while (stat != 0xb7f) {
        ptrace(PTRACE_CONT, targetPid, NULL, 0);
        waitpid(targetPid, &stat, WUNTRACED);
    }

    // detach and continue
    ptrace(PTRACE_SETREGS, targetPid, 0, &original_regs);
    ptrace(PTRACE_DETACH, targetPid, 0, (void *) SIGCONT);
    puts("Inject Finish");

    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_me_simple_lib_injection_RootUtil_callNative(JNIEnv *env, jclass clazz, jstring jarg1,
                                                           jstring jarg2) {
    jboolean isCopy = JNI_TRUE;
    const char *arg1 = env->GetStringUTFChars(jarg1, &isCopy);
    const char *arg2 = env->GetStringUTFChars(jarg1, &isCopy);
    doInject(arg1, arg2);
}
