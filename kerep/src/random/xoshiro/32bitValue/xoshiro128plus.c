/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoshiro128.h"

/* This is xoshiro128+ 1.0, our best and fastest 32-bit generator for 32-bit
   floating-poi32 numbers. We suggest to use its upper bits for
   floating-poi32 generation, as it is slightly faster than xoshiro128**.
   It passes all tests we are aware of except for
   linearity tests, as the lowest four bits have low linear complexity, so
   if low linear complexity is not considered an issue (as it is usually
   the case) it can be used to generate 32-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. */


static inline u32 rotl(const u32 x, i32 k) {
    return (x << k) | (x >> (32 - k));
}

u32 xoshiro128plus_next(void* _state){    
    xoshiro128_state* state=_state;
    const u32 result = state->s[0] + state->s[3];

    const u32 t = state->s[1] << 9;

    state->s[2] ^= state->s[0];
    state->s[3] ^= state->s[1];
    state->s[1] ^= state->s[2];
    state->s[0] ^= state->s[3];

    state->s[2] ^= t;

    state->s[3] = rotl(state->s[3], 11);

    return result;
}

void* xoshiro128_init(u64 seed){
    xoshiro128_state* state=malloc(sizeof(xoshiro128_state));
    splitmix64_state* splitmix=splitmix64_init(seed);
    state->merged[0]=splitmix64_next(splitmix);
    state->merged[1]=splitmix64_next(splitmix);
    splitmix64_free(splitmix);
    return state;
}
