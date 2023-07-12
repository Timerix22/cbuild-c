#pragma once

#if __cplusplus
extern "C" {
#endif

#include "std.h"

// returns length of char buffer (without \0)
u32 cptr_length(const char* str);

// allocates new char[] and copies src there
char* cptr_copy(const char* src);

bool cptr_equals(const char* key0, const char* key1);

bool cptr_startsWith(const char* src, const char* fragment);

bool cptr_endsWith(const char* src, const char* fragment);

// multiplies char n times
char* char_multiply(char c, u32 n);

/// @param startIndex 0 ... src length 
/// @param seekLength 0 ... -1
/// @return pos of first <fragment> inclusion in <src> or -1 if not found
i32 cptr_seek(const char* src, const char* fragment, u32 startIndex, u32 seekLength);

/// @param startIndex -1 ... src length
/// @param seekLength 0 ... -1
/// @return pos of first <fragment> inclusion in <src> or -1 if not found
i32 cptr_seekReverse(const char* src, const char* fragment, u32 startIndex, u32 seekLength);

/// @param startIndex 0 ... src length
/// @param seekLength 0 ... -1
/// @return pos of first <fragment> inclusion in <src> or -1 if not found
i32 cptr_seekChar(const char* src, char fragment, u32 startIndex, u32 seekLength);

/// @param startIndex -1 ... src length
/// @param seekLength 0 ... -1
/// @return pos of first <fragment> inclusion in <src> or -1 if not found
i32 cptr_seekCharReverse(const char* src, char fragment, u32 startIndex, u32 seekLength);

/// @brief search for <fragment> in <ptr>
/// @return index of first <fragment> inclusion or -1 if not found
static inline i32 cptr_indexOf(const char* src, const char* fragment)
{ return cptr_seek(src, fragment, 0, -1); }

/// @brief search for <fragment> in <ptr>
/// @return index of first <fragment> inclusion or -1 if not found
static inline i32 cptr_indexOfChar(const char* src, char fragment)
{ return cptr_seekChar(src, fragment, 0, -1); }

/// @brief search for <fragment> in <ptr>
/// @return index of last <fragment> inclusion or -1 if not found
static inline i32 cptr_lastIndexOf(const char* src, const char* fragment)
{ return cptr_seekReverse(src, fragment, -1, -1); }

/// @brief search for <fragment> in <ptr>
/// @return index of last <fragment> inclusion or -1 if not found
static inline i32 cptr_lastIndexOfChar(const char* src, char fragment)
{ return cptr_seekCharReverse(src, fragment, -1, -1); }


static inline bool cptr_contains(const char* src, const char* fragment){
    return cptr_seek(src, fragment, 0, -1) +1;
}

void memcopy(void* from, void* to, u32 size);

char* __cptr_concat(u32 n, ...);
#define cptr_concat(STR...) __cptr_concat(count_args(STR), STR)

char* cptr_toLower(const char* src);
char* cptr_toUpper(const char* src);

/// @param startIndex 0 ... src length
/// @param seekLength 0 ... -1
/// @return <src> with <str_old> replaced by <str_new> or empty cstring if <str_old> not found
char* cptr_replaceIn(const char* src, const char* str_old, const char* str_new, u32 startIndex, u32 seekLength);
/// @param startIndex 0 ... src length
/// @param seekLength 0 ... -1
/// @return <src> with <c_old> replaced by <c_new> or empty cstring if <str_old> not found
char* cptr_replaceCharIn(const char* src, char c_old, char c_new, u32 startIndex, u32 seekLength);

static inline char* cptr_replace(const char* src, const char* str_old, const char* str_new)
{ return cptr_replaceIn(src, str_old, str_new, 0, -1); }

static inline char* cptr_replaceChar(const char* src, char c_old, char c_new)
{ return cptr_replaceCharIn(src, c_old, c_new, 0, -1); }

#if __cplusplus
}
#endif