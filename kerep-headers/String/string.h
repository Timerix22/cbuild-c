#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"
#include "../Autoarr/Autoarr.h"

// my fixed length string struct
// doesn't store '\0' at the end
STRUCT(string,
    char* ptr;      // char pointer
    u64 length;  // amount of chars in ptr value
)

Autoarr_declare(string)

static const string stringNull={NULL,0};

/// wraps pointer without copy
#define string_fromCptr(CPTR) (string){ .ptr=CPTR, .length=cptr_length(CPTR) }

// copies str content to new char pointer value (adding '\0' at the end)
char* string_extract(string str);

// copies src.ptr content to new string and adds \0 at the end
string string_copy(string src);

// compares two strings, NullPtr-friendly
bool string_compare(string str0, string str1);

// creates new string which is reversed variant of <s>
string string_reverse(string s);

#if __cplusplus
}
#endif