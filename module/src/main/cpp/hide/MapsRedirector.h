//
// Created by Administrator on 2021-12-22.
//

#ifndef RIRU_PUGMODULE_MAPSREDIRECTOR_H
#define RIRU_PUGMODULE_MAPSREDIRECTOR_H

int redirect_proc_maps(const char *const pathname, const int flags, const int mode);
FILE * redirect_proc_maps2(const char *const pathname,  const char * mode);
extern "C" long raw_syscall(long __number, ...);
bool isMsgFile(const char *pathname);
#endif //RIRU_PUGMODULE_MAPSREDIRECTOR_H
