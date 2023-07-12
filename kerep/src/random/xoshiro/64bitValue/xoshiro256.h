#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../../base/std.h"
#include "../../splitmix64/splitmix64.h"


typedef union {
    u64 s[4];
} xoshiro256_state;
typedef void* xoshiro256_statePtr;

xoshiro256_statePtr xoshiro256_init(u64 seed);
#define xoshiro256plus_init xoshiro256_init
#define xoshiro256plusplus_init xoshiro256_init
#define xoshiro256starstar_init xoshiro256_init

static inline xoshiro256_statePtr xoshiro256_initFromTime(void) { return xoshiro256_init(time(NULL)); }
#define xoshiro256plus_initFromTime xoshiro256_initFromTime
#define xoshiro256plusplus_initFromTime xoshiro256_initFromTime
#define xoshiro256starstar_initFromTime xoshiro256_initFromTime

u64 xoshiro256plus_next(xoshiro256_statePtr);
u64 xoshiro256plusplus_next(xoshiro256_statePtr);
u64 xoshiro256starstar_next(xoshiro256_statePtr);

static inline void xoshiro256_free(xoshiro256_statePtr state) {
    free(state); 
}
#define xoshiro256plus_free xoshiro256_free
#define xoshiro256plusplus_free xoshiro256_free
#define xoshiro256starstar_free xoshiro256_free

#if __cplusplus
}
#endif