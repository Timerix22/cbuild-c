#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../Autoarr/Autoarr.h"
#include "string.h"

STRUCT(StringBuilder,
	Autoarr(string)* compl_bufs;
	Autoarr(i8)* curr_buf;
)

StringBuilder* StringBuilder_create(void);
void StringBuilder_free(StringBuilder* b);
void __StringBuilder_free(void* b);
// Joins all strings from compl_bufs.
// Returns zero-terminated string.
// No need to call string_extract()!
// Frees StringBuilder.
string StringBuilder_build(StringBuilder* b);
// removes last char
void StringBuilder_rmchar(StringBuilder* b);
void StringBuilder_append_char(StringBuilder* b, char c);
void StringBuilder_append_cptr(StringBuilder* b, char* s);
void StringBuilder_append_string(StringBuilder* b, string s);
void StringBuilder_append_i64(StringBuilder* b, i64 a);
void StringBuilder_append_u64(StringBuilder* b, u64 a);
void StringBuilder_append_f64(StringBuilder* b, f64 a);

#if __cplusplus
}
#endif