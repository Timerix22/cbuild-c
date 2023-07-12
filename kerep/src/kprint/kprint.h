#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/errors.h"
#include "kprint_format.h"

/*

This file looks like a mess, but all cotnent here just solves the problem of putting variadic arguments to array of formats and array of objects. 

*/

typedef union {
    i64 i64;
    u64 u64;
    f64 f64;
    void* ptr;
} __kp_value_union; 


static inline __kp_value_union __kpVU_f(f64 f) { return (__kp_value_union){ .f64=f }; } 
static inline __kp_value_union __kpVU_i(i64 f) { return (__kp_value_union){ .i64=f }; } 

#define __kpVU_selectType(V) _Generic(V, float: __kpVU_f, f64: __kpVU_f, default: __kpVU_i)(V)

#define __kpVU(V) __kpVU_selectType(V)

#define __kp_argsToFmts8( \
    a0, a1, a2, a3, a4, a5, a6, a7,...) \
    ((i32[]){ a0,a2,a4,a6 })
#define __kp_argsToObjs8( \
    a0, a1, a2, a3, a4, a5, a6, a7,...) \
    ((__kp_value_union[]){ __kpVU(a1),__kpVU(a3),__kpVU(a5),__kpVU(a7) })
    
#define __kp_argsToFmts16( \
    a0, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10,a11,a12,a13,a14,a15,...) \
    ((i32[]){ a0,a2,a4,a6,a8,a10,a12,a14 })
#define __kp_argsToObjs16( \
    a0, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10,a11,a12,a13,a14,a15,...) \
    ((__kp_value_union[]){ __kpVU(a1),__kpVU(a3),__kpVU(a5),__kpVU(a7),__kpVU(a9),__kpVU(a11),__kpVU(a13),__kpVU(a15) })

#define __kp_argsToFmts32( \
    a0, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10,a11,a12,a13,a14,a15, \
    a16,a17,a18,a19,a20,a21,a22,a23, \
    a24,a25,a26,a27,a28,a29,a30,a31,...) \
    ((i32[]){ a0,a2,a4,a6,a8,a10,a12,a14,a16,a18,a20,a22,a24,a26,a28,a30 })
#define __kp_argsToObjs32( \
    a0, a1, a2, a3, a4, a5, a6, a7, \
    a8, a9, a10,a11,a12,a13,a14,a15, \
    a16,a17,a18,a19,a20,a21,a22,a23, \
    a24,a25,a26,a27,a28,a29,a30,a31,...) \
    ((__kp_value_union[]){ __kpVU(a1),__kpVU(a3),__kpVU(a5),__kpVU(a7),__kpVU(a9),__kpVU(a11),__kpVU(a13),__kpVU(a15),__kpVU(a17),__kpVU(a19),__kpVU(a21),__kpVU(a23),__kpVU(a25),__kpVU(a27),__kpVU(a29),__kpVU(a31) })

#define __32zeroes 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

#define __kp_argsToArrs(COUNT,ARGS...) \
    (kp_fmt*)( \
        COUNT<=8 ? __kp_argsToFmts8(ARGS) : \
        COUNT<=16 ? __kp_argsToFmts16(ARGS) : \
        __kp_argsToFmts32(ARGS)), \
    (__kp_value_union*)( \
        COUNT<=8 ? __kp_argsToObjs8(ARGS) : \
        COUNT<=16 ? __kp_argsToObjs16(ARGS) : \
        __kp_argsToObjs32(ARGS))


Maybe __ksprint(u8 n, kp_fmt* formats, __kp_value_union* objects);

/// @param ARGS kp_fmt, value, kp_fmt, value...
///@returns Maybe<char*>
#define ksprint(ARGS...) WARNING_DISABLE( W_INT_CONVERSION, \
        __ksprint(count_args(ARGS), __kp_argsToArrs(count_args(ARGS),ARGS, __32zeroes)) \
    )
/*-Wint-conversion warning was produced during value to __kp_value_union conversion*/ 

Maybe __kfprint(FILE* fd, u8 n, kp_fmt* formats, __kp_value_union* objects);

/// @param FD FILE*  
/// @param ARGS kp_fmt, value, kp_fmt, value...
///@returns Maybe<void>
#define kfprint(FD, ARGS...) WARNING_DISABLE( W_INT_CONVERSION, \
        __kfprint(FD, count_args(ARGS), __kp_argsToArrs(count_args(ARGS),ARGS, __32zeroes)) \
    )

void __kprint(u8 n, kp_fmt* formats, __kp_value_union* objects);

///can use non-catchable throw !!!
///@param ARGS kp_fmt, value, kp_fmt, value...
///@returns void
#define kprint(ARGS...) WARNING_DISABLE( W_INT_CONVERSION, \
        __kprint(count_args(ARGS), __kp_argsToArrs(count_args(ARGS),ARGS, __32zeroes)) \
    )

#if __cplusplus
}
#endif