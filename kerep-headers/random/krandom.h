#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/std.h"
#include "splitmix64/splitmix64.h"
#include "xoroshiro/xoroshiro.h"
#include "xoshiro/xoshiro.h"

/*
You can choose any algorithm that has required functions:

    some_alg32_statePtr some_alg32_init(u32 seed);
    u32 some_alg32_next(some_alg32_statePtr);
    void some_alg32_free(some_alg32_statePtr);

    #define KRAND_ALG32_init some_alg32_init
    #define KRAND_ALG32_next some_alg32_next
    #define KRAND_ALG32_free some_alg32_free
    #include "kerep/random/krandom.h"

The same way it works for 64-bit RNGs
*/

// default rng_next function
#ifndef KRAND_ALG32_next
#define KRAND_ALG32_next xoshiro128plus##_next
#endif
#ifndef KRAND_ALG32_init
#define KRAND_ALG32_init xoshiro128plus##_init
#endif
#ifndef KRAND_ALG32_free
#define KRAND_ALG32_free xoshiro128plus##_free
#endif
#ifndef KRAND_ALG64_next
#define KRAND_ALG64_next xoshiro256plus##_next
#endif
#ifndef KRAND_ALG64_init
#define KRAND_ALG64_init xoshiro256plus##_init
#endif
#ifndef KRAND_ALG64_free
#define KRAND_ALG64_free xoshiro256plus##_free
#endif

typedef void* krand_statePtr;
#define KRAND_ALG32_initFromTime xoshiro128plus##_initFromTime
#define KRAND_ALG64_initFromTime xoshiro256plus##_initFromTime

#define __krand_next_definition(VALUE_SIZE) { return from+KRAND_ALG##VALUE_SIZE##_next(state)%(to-from); }

// ready-to-use functions
static inline i8  krand_next8 (krand_statePtr state, i8  from, i8  to) __krand_next_definition(32)
static inline i16 krand_next16(krand_statePtr state, i16 from, i16 to) __krand_next_definition(32)
static inline i32 krand_next32(krand_statePtr state, i32 from, i32 to) __krand_next_definition(32)
static inline i64 krand_next64(krand_statePtr state, i64 from, i64 to) __krand_next_definition(64)

// divides random number by 2^64 to return a value between 0 and 1
static inline f32 krand_nextFloat32(krand_statePtr state) {return (u32)KRAND_ALG32_next(state)/0xffffffff; }
static inline f64 krand_nextFloat64(krand_statePtr state) {return KRAND_ALG64_next(state)/0xffffffff; }


///@param chance (0-1.0) is probability of success
static inline bool fate(krand_statePtr state,float chance){
    i32 limit=1/chance + 0.01f;
    return KRAND_ALG32_next(state)%limit == 0;
}

#if __cplusplus
}
#endif