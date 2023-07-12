#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../errors.h"

// accepts char* (ptr to char) and char* (ptr to string)
// uses format kp_s and kp_c to determine what type is <c> argument
char* __toString_char(void* c, u32 fmt);

// bool
char* __toString_bool(void* c, u32 fmt);

// signed int
char* toString_i64(i64 n);
char* __toString_i8(void* n, u32 fmt);
char* __toString_i16(void* n, u32 fmt);
char* __toString_i32(void* n, u32 fmt);
char* __toString_i64(void* n, u32 fmt);

// unsigned int
char* toString_u64(u64 n, bool withPostfix, bool uppercase);
char* __toString_u8(void* n, u32 fmt);
char* __toString_u16(void* n, u32 fmt);
char* __toString_u32(void* n, u32 fmt);
char* __toString_u64(void* n, u32 fmt);

// float
#define toString_f32_max_precision 6
#define toString_f64_max_precision 15
#define toString_float_default_precision 6
char* toString_f32(f32 n, u8 precision, bool withPostfix, bool uppercase); // uses sprintf
char* toString_f64(f64 n, u8 precision, bool withPostfix, bool uppercase); // uses sprintf
char* __toString_f32(void* n, u32 fmt);
char* __toString_f64(void* n, u32 fmt); 


///@param inverse set to true for little endian numbers (their bytes are in reverse order)
char* toString_bin(void* bytes, u32 size, bool inverse, bool withPrefix);
///@param inverse set to true for little endian numbers (their bytes are in reverse order)
char* toString_hex(void* bytes, u32 size, bool inverse, bool withPrefix, bool uppercase);

#if __cplusplus
}
#endif