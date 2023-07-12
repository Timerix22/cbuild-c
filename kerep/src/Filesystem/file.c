#include "filesystem.h"
#include "../String/StringBuilder.h"
#include "io_includes.h"

void __file_freeMembers(void* _f){ fclose((FileHandle)_f); }

kt_define(FileHandle, __file_freeMembers, NULL)

bool file_exists(const char* path){
    if(path[0]=='.'){
        if(path[1]==0 || (path[1]==path_sep && path[2]==0))
            return false; // . or ./ is not a file
        // else if(path[1]=='.' && path[2]==path_sep)
        //TODO path_resolve because windows doesnt recognize .\ pattern
    }

#if KFS_USE_WINDOWS_H
    DWORD dwAttrib = GetFileAttributes(path);
    return (bool)(
        (dwAttrib != INVALID_FILE_ATTRIBUTES) && // file exists
        !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)); // file is not directory
#else
    struct stat stats;
    i32 rez=stat(path, &stats);
    return (bool)(
        (rez!=-1) && // file exists
        !(S_ISDIR(stats.st_mode))); // file is not directory
#endif
}

Maybe file_delete(const char* path, bool recursive){
    throw(ERR_NOTIMPLEMENTED);
    return MaybeNull;
}

char* FileOpenMode_toStr(FileOpenMode m){
    char* p;
    switch(m){
        case FileOpenMode_Read: p="rb"; break;
        case FileOpenMode_Write: p="wb"; break;
        case FileOpenMode_Append: p="ab"; break;
        case FileOpenMode_ReadWrite: p="wb+"; break;
        case FileOpenMode_ReadAppend: p="ab+"; break;
        default:
            dbg(m);
            throw(ERR_UNEXPECTEDVAL);
    }
    return p;
}

Maybe file_open(const char* path, FileOpenMode mode){
    FileHandle file=fopen(path, FileOpenMode_toStr(mode));
    if(!file)
        safethrow(cptr_concat("can't open file ", (char*)path),;);
    return SUCCESS(UniHeapPtr(FileHandle,file));
}

Maybe file_close(FileHandle file){
    if(!file)
        safethrow(ERR_NULLPTR,;);
    if(fclose(file))
        safethrow(ERR_IO,;);
    return MaybeNull;
}

#define ioWriteCheck() \
    if(rezult==EOF) \
        safethrow(ERR_IO_EOF,;); \
    if(rezult!=0) \
        safethrow(ERR_IO,;);

Maybe file_writeChar(FileHandle file, char byte){
    i32 rezult=fputc(byte, file);
    ioWriteCheck();
    return MaybeNull;
}

Maybe file_writeBuffer(FileHandle file, char* buffer, u64 length){
    i32 rezult=0;
    for(u64 i=0; i<length && !rezult; i++)
        rezult=fputc(buffer[i], file);
    ioWriteCheck();
    return MaybeNull;
}

Maybe file_writeCptr(FileHandle file, char* cptr){
    i32 rezult=fputs(cptr, file);
    ioWriteCheck();
    return MaybeNull;
}


Maybe file_readChar(FileHandle file){
    i32 rezult=fgetc(file);
    if(feof(file)) safethrow(ERR_IO_EOF,;);
    if(ferror(file)) safethrow(ERR_IO,;);
    return SUCCESS(UniUInt64(rezult));
}

Maybe file_readBuffer(FileHandle file, char* buffer, u64 length){
    i32 rezult=0;
    u64 i=0;
    for(; i<length && rezult!=EOF; i++){
        rezult=fgetc(file);
        buffer[i]=(char)rezult;
    }
    if(ferror(file)) safethrow(ERR_IO,;);
    return SUCCESS(UniUInt64(i));
}

Maybe file_readAll(FileHandle file, char** allBytes){
    i32 rezult=0;
    char buffer[256];
    string bufStr={.ptr=buffer, .length=sizeof(buffer)};
    StringBuilder* sb=StringBuilder_create();
    u64 i=0;
    while(true){
        rezult=fgetc(file);
        if(rezult==EOF){
            if(ferror(file)) 
                safethrow(ERR_IO, StringBuilder_free(sb));
            break;
        }
        buffer[i%sizeof(buffer)]=(char)rezult;
        i++;
        if(!(i%sizeof(buffer)))
            StringBuilder_append_string(sb,bufStr);
    }
    bufStr.length=i%sizeof(buffer);
    if(bufStr.length!=0)
        StringBuilder_append_string(sb,bufStr);
    string str=StringBuilder_build(sb);
    *allBytes=str.ptr;
    // i dont know how can it happen, but if it will have, it will be very dangerous
    if(i!=str.length) 
        throw(ERR_UNEXPECTEDVAL);
    return SUCCESS(UniUInt64(i));
}
