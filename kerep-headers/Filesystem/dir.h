#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"
#include "file.h"

bool dir_exists(const char* path);
///@return Maybe<void>
Maybe dir_create(const char* path);
///@return Maybe<void>
Maybe dir_delete(const char* path);

///@return Maybe<Array_string>
Maybe dir_getFiles(const char* path, bool recursive);
///@return Maybe<Array_string>
Maybe dir_getDirs(const char* path, bool recursive);

///@return Maybe<Array_string>
Maybe dir_findFiles(const char* path, char* searchPattern, bool recursive);
///@return Maybe<Array_string>
Maybe dir_findDirs(const char* path, char* searchPattern, bool recursive);

#if __cplusplus
}
#endif