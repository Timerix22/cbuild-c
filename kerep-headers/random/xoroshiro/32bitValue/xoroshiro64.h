#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../../base/std.h"
#include "../../splitmix64/splitmix64.h"

typedef union {
    u64 merged;
    u32 s[2];
} xoroshiro64_state;
typedef void* xoroshiro64_statePtr;

xoroshiro64_statePtr xoroshiro64_init(u64 seed);
#define xoroshiro64star_init xoroshiro64_init
#define xoroshiro64starstar_init xoroshiro64_init

static inline xoroshiro64_statePtr xoroshiro64_initFromTime(void) { return xoroshiro64_init(time(NULL)); }
#define xoroshiro64star_initFromTime xoroshiro64_initFromTime
#define xoroshiro64starstar_initFromTime xoroshiro64_initFromTime

u32 xoroshiro64star_next(xoroshiro64_statePtr);
u32 xoroshiro64starstar_next(xoroshiro64_statePtr);

static inline void xoroshiro64_free(xoroshiro64_statePtr state) {
    free(state); 
}
#define xoroshiro64star_free xoroshiro64_free
#define xoroshiro64starstar_free xoroshiro64_free

#if __cplusplus
}
#endif