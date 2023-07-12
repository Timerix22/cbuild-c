#include "../base/std.h"

#if defined(_WIN64) || defined(_WIN32)
#define KFS_USE_WINDOWS_H 1
#else 
#define KFS_USE_WINDOWS_H 0
#endif

#if KFS_USE_WINDOWS_H
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
