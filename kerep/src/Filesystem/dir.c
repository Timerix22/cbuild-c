#include "filesystem.h"
#include "io_includes.h"
#include "../kprint/kprint.h"

bool dir_exists(const char* path){
    if(path[0]=='.'){
        if(path[1]==0 || (path[1]==path_sep && path[1]==0)) 
            return true; // dir . or ./ always exists
        // else if(path[1]=='.' && path[2]==path_sep)
        //TODO path_resolve because windows doesnt recognize .\ pattern
    }
#if KFS_USE_WINDOWS_H
    DWORD dwAttrib = GetFileAttributes(path);
    return (bool)(
        (dwAttrib != INVALID_FILE_ATTRIBUTES) && // file exists
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)); // file is a directory
#else
    struct stat stats;
    i32 rez=stat(path, &stats);
    return (bool)(
        (rez!=-1) && // file exists
        (S_ISDIR(stats.st_mode))); // file is a directory
#endif
}

Maybe dir_create(const char* path){
    if (dir_exists(path))
        return MaybeNull;
    char* parentDir=path_parentDir(path);
    dir_create(parentDir);
    free(parentDir);
#if KFS_USE_WINDOWS_H
    if(!CreateDirectory(path, NULL))
#else
    if(mkdir(path, 0777) == -1)
#endif
    {
        char err[512];
        IFWIN(
            sprintf_s(err, 512, "can't create dicectory <%s>", path),
            sprintf(err, "can't create dicectory <%s>", path));
        safethrow(err,;);
    }

    return MaybeNull;
}

Maybe dir_delete(const char* path){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}

Maybe dir_getFiles(const char* path, bool recursive){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}
Maybe dir_getDirs(const char* path, bool recursive){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}

Maybe dir_findFiles(const char* path, char* searchPattern, bool recursive){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}
Maybe dir_findDirs(const char* path, char* searchPattern, bool recursive){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}
