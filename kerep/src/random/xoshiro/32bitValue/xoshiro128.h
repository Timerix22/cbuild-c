#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../../base/std.h"
#include "../../splitmix64/splitmix64.h"


typedef union {
    u64 merged[2];
    u32 s[4];
} xoshiro128_state;
typedef void* xoshiro128_statePtr;

xoshiro128_statePtr xoshiro128_init(u64 seed);
#define xoshiro128plus_init xoshiro128_init
#define xoshiro128plusplus_init xoshiro128_init
#define xoshiro128starstar_init xoshiro128_init

static inline xoshiro128_statePtr xoshiro128_initFromTime(void) { return xoshiro128_init(time(NULL)); }
#define xoshiro128plus_initFromTime xoshiro128_initFromTime
#define xoshiro128plusplus_initFromTime xoshiro128_initFromTime
#define xoshiro128starstar_initFromTime xoshiro128_initFromTime

u32 xoshiro128plus_next(xoshiro128_statePtr);
u32 xoshiro128plusplus_next(xoshiro128_statePtr);
u32 xoshiro128starstar_next(xoshiro128_statePtr);

static inline void xoshiro128_free(xoshiro128_statePtr state) {
    free(state); 
}
#define xoshiro128plus_free xoshiro128_free
#define xoshiro128plusplus_free xoshiro128_free
#define xoshiro128starstar_free xoshiro128_free

#if __cplusplus
}
#endif