#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"
#include "../String/string.h"

typedef FILE* FileHandle;
kt_declare(FileHandle);

bool file_exists(const char* path);

///@return Maybe<void>
Maybe file_delete(const char* path, bool recursive);

PACKED_ENUM(FileOpenMode,
    // open a file for reading 
    FileOpenMode_Read=1, 
    // (re)create a file for writing
    FileOpenMode_Write=2, 
    // opens file for writing additional data to the end / creates new file 
    FileOpenMode_Append=4,
    // (re)creates file for reading/writing
    FileOpenMode_ReadWrite=FileOpenMode_Read|FileOpenMode_Write, 
    // opens file for readng/writing additional data to the end / creates new file 
    FileOpenMode_ReadAppend=FileOpenMode_Read|FileOpenMode_Append
)

/// @brief opens file
/// @param path path to file
/// @param mode Read/Write/Append/ReadWrite/ReadAppend
/// @return Maybe<FileHandle>
Maybe file_open(const char* path, FileOpenMode mode);

/// @brief closes file descriptor
/// @return Maybe<void>
Maybe file_close(FileHandle file);

/// @brief closes file descriptor
/// @param byte byte to write
/// @return Maybe<void>
Maybe file_writeChar(FileHandle file, char byte);

/// @brief closes file descriptor
/// @param buffer bytes to write
/// @param length buffer length
/// @return Maybe<void>
Maybe file_writeBuffer(FileHandle file, char* buffer, u64 length);

/// @brief writes all cstring array content to file
/// @param cptr zero-terminated cstring
/// @return Maybe<void>
Maybe file_writeCptr(FileHandle file, char* cptr);


/// @brief reads single byte from file
/// @return Maybe<char>
Maybe file_readChar(FileHandle file);

/// @brief reads byte array of specofied length
/// @param buffer buffer that will be filled with file bytes
/// @param length buffer length
/// @return Maybe<u64> total number of successfully read bytes (<=length)
Maybe file_readBuffer(FileHandle file, char* buffer, u64 length);

/// @brief reads all bytes from file
/// @param allBytes ptr to the file's content will be pushed there 
/// @return Maybe<u64> total number of successfully read bytes
Maybe file_readAll(FileHandle file, char** allBytes);

#if __cplusplus
}
#endif