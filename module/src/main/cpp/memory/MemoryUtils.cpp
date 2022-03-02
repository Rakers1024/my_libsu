//
// Created by admin on 2022/2/19.
//

#include <dirent.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <asm/fcntl.h>
#include <zconf.h>
#include "MemoryUtils.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>
#include <dirent.h>
#include "Structs.h"
#include "../include/log.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <zconf.h>
#include <fcntl.h>

//获取进程pid
int get_pid_by_packagename(const char *szPro);
/* 读取模块入口地址 */
uintptr_t get_module_base(const char *module_name,int mPid);
uintptr_t getModule(const char *lib, bool isStart);
char *my_strcpy(char *dest, const char *src);
void *my_memmove(void *dst, const void *src, size_t count);
char *my_strcpy(char *dest, const char *src);
void writeBuffer(uintptr_t address, void *buffer, int size);
//缺页判断
int isMincore(uintptr_t _addr, size_t _size);

bool memoryRead(uintptr_t address, void *buffer, int size);
float getF(uintptr_t address);
uintptr_t getA(uintptr_t address);
char getChar(uintptr_t address);
int getI(uintptr_t address);
void writeFloat(uintptr_t address, float value);
void writeByte(uintptr_t address, int value);
void writeInt(uintptr_t address, int value);
//字节查找
uintptr_t FindPattern(const char *lib, const char *pattern);

void *my_memmove_fun(void *dst, const void *src, size_t count) {
    // 容错处理
    if (src == NULL || dst == NULL) {
        return NULL;
    }
    unsigned char *pdst = (unsigned char *) dst;
    const unsigned char *psrc = (const unsigned char *) src;
    if (psrc == NULL) {
        return NULL;
    }
    //判断内存是否重叠
    bool flag1 = (pdst >= psrc && pdst < psrc + count);
    bool flag2 = (psrc >= pdst && psrc < pdst + count);
    if (flag1 || flag2) {
        // 倒序拷贝
        while (count) {
            *(pdst + count - 1) = *(psrc + count - 1);
            count--;
        }//while
    } else {
        // 拷贝
        while (count--) {
            *pdst = *psrc;
            pdst++;
            psrc++;
        }//while
    }
    return dst;
}
void *my_memmove(void *dst, const void *src, size_t count) {
    if (src == NULL || dst == NULL) {
        return NULL;
    }
    char *tmp_dst = (char *) dst;
    char *tmp_src = (char *) src;
    if (tmp_src == NULL) {
        return NULL;
    }
    if (tmp_dst + count < src || tmp_src + count < dst) {
        while (count--)
            *tmp_dst++ = *tmp_src++;
    } else {
        tmp_dst += count - 1;
        tmp_src += count - 1;
        while (count--)
            *tmp_dst-- = *tmp_src--;
    }

    return dst;
}
void *my_memcpy(void *dst, const void *src, size_t count) {
    if (src == NULL || dst == NULL) {
        return NULL;
    }
    char *tmp_dst = (char *) dst;
    char *tmp_src = (char *) src;
    if (tmp_src == NULL) {
        return NULL;
    }
    while (count--)
        *tmp_dst++ = *tmp_src++;

    return dst;
}
char *my_strcpy(char *dest, const char *src) {
    //assert((dest != NULL) && (src != NULL));
    if (src == NULL || dest == NULL) {
        return NULL;
    }
    const char *end = src;
    while (*end)
        end++;
    my_memcpy(dest, src, end - src + 1);
    return dest;
}
char *my_strcpy2(char *strDest, const char *strSrc) {
    if (strDest == NULL || strSrc == NULL)
        return NULL;
    if (strDest == strSrc)
        return strDest;
    char *tempDest = strDest;
    while ((*strDest++ = *strSrc++) != '\0');
    return tempDest;
}
/* 获取进程PID */
int get_pid_by_packagename(const char *szPro) {
    struct dirent *ptr = NULL;
    char cmdline[256], szName[256];
    DIR *dir = opendir("/proc");
    if (NULL != dir) {
        while ((ptr = readdir(dir)) != NULL) {
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)) continue;
            if (ptr->d_type != DT_DIR) continue;
            sprintf(cmdline, "/proc/%s/cmdline", ptr->d_name);
            FILE *fp = fopen(cmdline, "r");
            if (NULL != fp) {
                fgets(szName, sizeof(szName), fp);
                if (strcmp(szName, szPro) == 0) { break; }
                fclose(fp);
            }
        }
    }
    if (readdir(dir) == NULL) {
        return 0;
    }
    closedir(dir);
    return atoi(ptr->d_name);
}

/* 读取模块入口地址 */
uintptr_t get_module_base(const char *module_name,int mPid) {
    FILE *fp;
    uintptr_t addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", mPid);
    //sprintf(filename, "%s", "/proc/self/maps");

    fp = fopen(filename, "rt");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
#if defined(__LP64__)
                sscanf(buffer, "%lx-%*s", &addr);
#else
                sscanf(buffer, "%x-%*s", &addr);
#endif
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

uintptr_t getModule(const char *moduleName, bool isStart) {
    char mapsPath[64] = "";
    sprintf(mapsPath, "%s", "/proc/self/maps");
    int fd = open(mapsPath, O_RDONLY);
    LOGD("mapsFd = %d", fd);
    uintptr_t start = 0;
    uintptr_t end = 0;
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
                        //start = (uintptr_t)strtoull(p, NULL, 16);

                        firstFind = false;
                    }
                } else {
#ifdef __LP64__
                    end = strtoul(p + 11, NULL, 16);
#else
                    end = strtoul(p + 9, NULL, 16);
#endif
                    //find end

                }
            }
            p = e + 1;
        }
        if (p == line) { // !any_entry
            goto __break;
        } //if

        const size_t remain = strlen(p);
        if (remain <= (PATH_MAX / 2)) {
            my_memcpy(line, p, remain * sizeof(p[0]));
        } else {
            my_memmove(line, p, remain * sizeof(p[0]));
        } //if

        p = line + remain;
        n = PATH_MAX - 1 - remain;
    }

    __break:
    close(fd);
    return isStart ? start : end;

}

bool memoryRead(uintptr_t address, void *buffer, int size) {
    memset(buffer, 0, size);
#ifdef __LP64__
    if (address <= 0x10000000 || address >=0x10000000000) {
        return false;
    }
#else
    if (address <= 0x100000  || address >= 0x10000000) {
        return false;
    }
#endif
    // if (!isMincoreSafeAddr(address)) { return false; }

    if (isMincore(address, size) != 1) { return false; }
    // return pvm(reinterpret_cast<void *>(address), buffer, size, false, mPid) ;
    return my_memmove(buffer, reinterpret_cast<void *>(address), size) != NULL;
}

void writeBuffer(uintptr_t address, void *buffer, int size) {
#ifdef __LP64__

    if (address <= 0x100000 || address % 4 != 0) {
        //  return false;
    }
    if (address <= 0x10000000) {
        return;
    }
#else
    if (address <= 0x100000 || address % 4 != 0) {
        //  return ;
    }
    if (address <= 0x100000) {
        return;
    }
#endif
    //修改内存属性
    uintptr_t _addr = address & (~(PAGE_SIZE - 1));
    mprotect((void *) _addr, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (isMincore(address, size) == 1) {
        my_memmove((void *) address, buffer, size);
    }
}
#include "MapsRedirector.h"
int isMincore(uintptr_t _addr, size_t _size) {
    uintptr_t address = _addr & (~(PAGE_SIZE - 1));
    size_t size = _addr - address + _size;
    size % PAGE_SIZE ? size += PAGE_SIZE - size % PAGE_SIZE : size;
    int mincore_size = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    unsigned char mincore_buffer[10];// = (unsigned char *) malloc(mincore_size);
    if (mincore_size > sizeof(mincore_buffer)) return -1; //好了
    int ret = raw_syscall(__NR_mincore, (void *) address, size, mincore_buffer);
    if (ret != 0) {
        //free(mincore_buffer); //这里面有锁， 速度很慢， 中间间隔就很容易被检测
        return ret;
    }
    ret = 1;
    for (size_t i = 0; i < mincore_size; i++) {
        if ((mincore_buffer[i] & 1) != 1)
            ret = 0;
    }
    //free(mincore_buffer);
    return ret;
}


int getI(uintptr_t address) {
    int value = 0;
    int *p = &value;
    if (!memoryRead(address, p, sizeof(int))) {
        return 0;
    }
    return value;
}

uintptr_t getA(uintptr_t address) {
    uintptr_t value = 0;
    uintptr_t *p = &value;
    if (!memoryRead(address, p, sizeof(uintptr_t))) {
        return 0;
    }
#ifdef __LP64__
    if (address <= 0x10000000 || address >= 0x10000000000) {
        return false;
    }
#else
    if (address <= 0x100000  || address >= 0x10000000) {
        return false;
    }
#endif
    return value;
}


float getF(uintptr_t address) {
    float value = 0.0f;
    float *p = &value;
    if (!memoryRead(address, p, sizeof(float))) {
        return 0.0;
    }
    return value;
}

char getChar(uintptr_t address) {
    char value;
    char *p = &value;
    if (!memoryRead(address, p, sizeof(char))) {
        return '\0';
    }
    return value;
}


void writeFloat(uintptr_t address, float value) {
    if (getF(address) != value) {
        writeBuffer(address, &value, sizeof(value));
    }
}

void writeByte(uintptr_t address, int value) {
    writeBuffer(address, &value, 1);
}



void writeInt(uintptr_t address, int value) {
    if (getI(address) != value) {
        writeBuffer(address, &value, sizeof(value));
    }
}



int fd2 = -1;

int isAddressInPhysical(unsigned long vmAddr) {
    int pageSize = getpagesize();
    unsigned long v_pageIndex = vmAddr / pageSize;   // 虚地址页数
    unsigned long v_offset = v_pageIndex * sizeof(uint64_t); // 计算在/proc/pid/page_map文件中的偏移量
    unsigned long page_offset = vmAddr % pageSize;   // 计算虚拟地址在页面中的偏移量
    uint64_t item = 0;          // 存储对应项的值
    if (fd2 == -1) {
        char *fileName  = (char *) malloc(64);
       // my_strcpy(fileName, "/proc/self/pagemap");
        fd2 = open(fileName, O_RDONLY);
        free(fileName);
        fileName = NULL;
    }
    pread(fd2, &item, sizeof(uint64_t), v_offset);
    if ((((uint64_t) 1 << 63) & item) != 0) {
        return 1;
    }
    return 0;
}

bool hasPfn(uint64_t data) {
    return (data & 0x7fffffffffffff) != 0;
}

#define    PFN_PRESENT_FLAG  (((uint64_t)1)<<63)

bool isPresent(uint64_t data) {
    return (data & PFN_PRESENT_FLAG) != 0;
};

#define    PFN_SWAP_FLAG  (((uint64_t)1)<<62)

bool isSWAPED(uint64_t data) {
    return (data & PFN_SWAP_FLAG) != 0;
};


#define INRANGE(x, a, b)        (x >= a && x <= b)
#define getBits(x)              (INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte(x)              (getBits(x[0]) << 4 | getBits(x[1]))

uintptr_t FindPattern(const char *lib, const char *pattern) {

    uintptr_t start;// = getModuleAddr("libtersafe.so", true);
    uintptr_t end;//= getModuleAddr("libtersafe.so", false);
    char filename[32], buffer[1024];
    // string r = "cp -f  /proc/self/maps /storage/emulated/0/maps";
    //   system(r.c_str());
    //   snprintf(filename, sizeof(filename), "/storage/emulated/0/maps");
    snprintf(filename, sizeof(filename), "/proc/self/maps");

    FILE *fp = fopen(filename, "r-xp");
    while (!feof(fp)) {
        fgets(buffer, sizeof(buffer), fp);
        if (strstr(buffer, lib)) {
#if defined(__LP64__)
            sscanf(buffer, "%lx-%lx %*s", &start, &end);
#else
            sscanf(buffer, "%x-%x %*s", &start,&end);
#endif
            break;
        }
    }
    //  system("rm - r /storage/emulated/0/maps");

    fclose(fp);

    //  auto start = getModuleAddr(lib,true);
    if (!start)
        return 0;

    //  auto end =  getModuleAddr(lib,false);
    if (!end)
        return 0;

    auto curPat = reinterpret_cast<const unsigned char *>(pattern);
    uintptr_t firstMatch = 0;
    for (uintptr_t pCur = start; pCur < end; ++pCur) {
        if (*(uint8_t *) curPat == (uint8_t) '\?' || *(uint8_t *) pCur == getByte(curPat)) {
            if (!firstMatch) {
                firstMatch = pCur;
            }
            curPat += (*(uint16_t *) curPat == (uint16_t) '\?\?' ||
                       *(uint8_t *) curPat != (uint8_t) '\?') ? 2 : 1;
            if (!*curPat) {
                LOGD("my_loc:%lx lib+offset:%lx ", firstMatch, firstMatch - start);
                return firstMatch;
            }
            curPat++;
            if (!*curPat) {
                LOGD("my_loc:%lx lib+offset:%lx ", firstMatch, firstMatch - start);

                return firstMatch;
            }
        } else if (firstMatch) {
            pCur = firstMatch;
            curPat = reinterpret_cast<const unsigned char *>(pattern);
            firstMatch = 0;
        }
    }
    return 0;
}

HashMap validateMap[MaxValidateCount];
int validateCount = 0;


void clearMap() {
    validateCount = 0;
    memset(validateMap, 0, (MaxValidateCount) * sizeof(HashMap));

}

/*
int new_mincore(void* __addr, size_t __size, unsigned char *mincore_buffer) {
    __addr = reinterpret_cast<void *>(0x123456);
    return orig_mincore( (void *) __addr, __size, mincore_buffer);
}
*/
int (*orig_mincore)(void *__addr, int size, unsigned char *mincore_buffer);
int new_mincore(void* __addr, size_t __size, unsigned char *mincore_buffer) {
    bool isHave = false;
    for (int i = 0; i < validateCount; i++) {
        if (reinterpret_cast<void *>(validateMap[i].addr) == __addr) {
            if (validateCount >= MaxValidateCount) goto _break;
            //覆盖数据
            validateMap[i].addr = reinterpret_cast<uintptr_t>(__addr);
            validateMap[i].size = __size;
            isHave = true;
        }
    }

    if (!isHave) {
        if (validateCount >= MaxValidateCount) goto _break;
        //新数据
        validateMap[validateCount].addr = reinterpret_cast<uintptr_t>(__addr);
        validateMap[validateCount].size = __size;
        validateCount++;
    }
    goto _break;
    _break:
    return orig_mincore( (void *) __addr, __size, mincore_buffer);
}

int (*orig_madvise)(void *__addr, size_t size, int __advice);

int new_madvise(void* __addr, size_t __size, int __advice){
    LOGD("hook madvise %lx: size:%zu advice:%d",reinterpret_cast<uintptr_t>(__addr),__size,__advice);
    return 0;
}

int isMincoreSafeAddr(uintptr_t address) {
    for (int i = 0; i < validateCount; i++) {
        if (address >= validateMap[i].addr && address <= (validateMap[i].addr + validateMap[i].size)) {
            if (isMincore(address, validateMap[i].size) != 1) {
                LOGD("读到检测: addr:%lx fack_addr:%lx size:%zu my_mincore:%d", address,
                     validateMap[i].addr, validateMap[i].size,
                     isMincore(address, validateMap[i].size));
                return 0;
            }
        }
    }
    return 1;
}
/*
#include "whale.h"
void Hook(void *target, void *replace, void **backup) {
    unsigned long page_size = sysconf(_SC_PAGESIZE);
    unsigned long size = page_size * sizeof(uintptr_t);
    if (mprotect((void *) ((uintptr_t) target - ((uintptr_t) target % page_size) - page_size), (size_t) size, PROT_EXEC | PROT_READ | PROT_WRITE) == 0) {
        WInlineHookFunction(target, replace, backup);
    }
}*/