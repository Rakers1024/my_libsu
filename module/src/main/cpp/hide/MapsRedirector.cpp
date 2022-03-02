//
// Created by Administrator on 2021-12-22.
//
#include <cstring>
#include <cstdio>
#include <climits>
#include <unistd.h>
#include <cstdlib>
#include <syscall.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <log.h>

#include "MapsRedirector.h"
//std::string path;


static int create_temp_file() {
    int fd = -1;
    char *path = nullptr;
    if (asprintf(&path, "/data/user/0/com.tencent.tmgp.pubgmhd/dev_maps_%d_%d", getpid(),
                 gettid()) == -1) {
        return -1;
    }
    fd = (int) raw_syscall(__NR_openat, AT_FDCWD, path, O_RDWR | O_CLOEXEC | O_TMPFILE | O_EXCL,
                           nullptr);
    if (fd != -1) {
        goto __end;
    }
    fd = (int) raw_syscall(__NR_openat, AT_FDCWD, path, O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC,
                           nullptr);
    __end:
    unlink(path);
    free(path);
    return fd;
}

static void *my_memmove(void *dst, const void *src, size_t count) {
    if (src == nullptr || dst == nullptr) {
        return nullptr;
    }
    char *tmp_dst = (char *) dst;
    char *tmp_src = (char *) src;
    if (tmp_src == nullptr) {
        return nullptr;
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


static void *my_memcpy(void *dst, const void *src, size_t count) {
    if (src == nullptr || dst == nullptr) {
        return nullptr;
    }
    char *tmp_dst = (char *) dst;
    char *tmp_src = (char *) src;
    if (tmp_src == nullptr) {
        return nullptr;
    }
    while (count--)
        *tmp_dst++ = *tmp_src++;

    return dst;
}

bool isMsgFile(const char *pathname) {
    // 如果前缀是内置存储目录的，且 目录内容没有 com.tencent.tmgp.pubgmhd 包名的，都过滤掉
    if ((strstr(pathname, "/storage") || strstr(pathname, "/sdcard")) && strstr(pathname, "com.tencent.tmgp.pubgmhd") == nullptr && strstr(pathname,"/imgui_config") == nullptr)
        return true;
    return false;
}

// 过滤的东西
static bool MatchMapsItem(const char *path) {
    return isMsgFile(path) || // 过滤除了游戏资源以外的文件
           strstr(path, "magisk") != nullptr ||
           strstr(path, "lsposed") != nullptr ||
           strstr(path, "xposed") != nullptr ||
           strstr(path, "qnotified") != nullptr ||
           strstr(path, "qn_dyn_lib") != nullptr ||
           strstr(path, "qn_mmkv") != nullptr ||
           strstr(path, "hook") != nullptr ||
           strstr(path, "xposedhook") != nullptr ||
           (strlen(path) < 45 && (strstr(path, "rwx") || strstr(path, "r-x")))// 排除匿名内存的可执行段
            ;
}

static void redirect_proc_maps_internal(const int fd, const int fake_fd) {
    char line[PATH_MAX];
    char *p = line, *e;
    size_t n = PATH_MAX - 1;
    ssize_t r;
    while ((r = TEMP_FAILURE_RETRY(read(fd, p, n))) > 0) {
        p[r] = '\0';
        p = line;
        while ((e = strchr(p, '\n')) != nullptr) {
            e[0] = '\0';
            if (MatchMapsItem(p)) {
                //  LOGE("[maps] delete line: %s", p);
            } else {
                e[0] = '\n';
                write(fake_fd, p, e - p + 1);
            }
            p = e + 1;
        }

        if (p == line) { // !any_entry
            goto __break;
        }

        const size_t remain = strlen(p);
        if (remain <= (PATH_MAX / 2)) {
            my_memcpy(line, p, remain * sizeof(p[0]));
        } else {
            my_memmove(line, p, remain * sizeof(p[0]));
        }
        p = line + remain;
        n = PATH_MAX - 1 - remain;
    }
    __break:
    return;
}

int redirect_proc_maps(const char *const pathname, const int flags, const int mode) {
    if (strncmp(pathname, "/proc/", sizeof("/proc/") - 1) != 0) {
        return 0;
    }
    const char *s = pathname + sizeof("/proc/") - 1;
    const char *p = strstr(s, "/maps");
    const char *p1 = strstr(s, "/smaps");
    if (p == nullptr && p1 == nullptr) {
        return 0;
    }
    int fd = (int) raw_syscall(__NR_openat, AT_FDCWD, pathname, 0, mode);
    if (fd == -1) {
        return -1;
    }
    //创建一个临时Maps
    int fake_fd = create_temp_file();
    if (fake_fd == -1) {
        return -1;
    }
    //往Maps里面写 过滤掉我们需要过滤的内容
    redirect_proc_maps_internal(fd, fake_fd);
    lseek(fake_fd, 0, SEEK_SET);
    raw_syscall(__NR_close, fd);
    return fake_fd;
}

FILE *redirect_proc_maps2(const char *pathname, const char *mode) {
    if (strncmp(pathname, "/proc/", sizeof("/proc/") - 1) != 0) {
        return nullptr;
    }
    const char *s = pathname + sizeof("/proc/") - 1;
    const char *p = strstr(s, "/maps");
    const char *p1 = strstr(s, "/smaps");
    if (p == nullptr && p1 == nullptr) {
        return nullptr;
    }
    int fd = (int) raw_syscall(__NR_openat, AT_FDCWD, pathname, 0, mode);
    if (fd == -1) {
        return nullptr;
    }
    //创建一个临时Maps
    int fake_fd = -1;
    char *path = nullptr;
    if (asprintf(&path, "/data/user/0/com.tencent.tmgp.pubgmhd/dev_maps_%d_%d", getpid(),
                 gettid()) == -1) {
        return nullptr;
    }
    fake_fd = (int) raw_syscall(__NR_openat, AT_FDCWD, path,
                                O_RDWR | O_CLOEXEC | O_TMPFILE | O_EXCL, nullptr);
    if (fake_fd == -1) {
        fake_fd = (int) raw_syscall(__NR_openat, AT_FDCWD, path,
                                    O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC, nullptr);
    }
    //往Maps里面写 过滤掉我们需要过滤的内容
    redirect_proc_maps_internal(fd, fake_fd);
    raw_syscall(__NR_close, fd);
    FILE *f = fopen(path, mode);
    //删除文件
    unlink(path);
    free(path);
    return f;
}

