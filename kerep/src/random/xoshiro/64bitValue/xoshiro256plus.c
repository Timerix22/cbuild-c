/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoshiro256.h"

/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
   numbers. We suggest to use its upper bits for floating-point
   generation, as it is slightly faster than xoshiro256++/xoshiro256**. It
   passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline u64 rotl(const u64 x, i32 k) {
    return (x << k) | (x >> (64 - k));
}

u64 xoshiro256plus_next(void* _state){    
    xoshiro256_state* state=_state;
    const u64 result = state->s[0] + state->s[3];

    const u64 t = state->s[1] << 17;

    state->s[2] ^= state->s[0];
    state->s[3] ^= state->s[1];
    state->s[1] ^= state->s[2];
    state->s[0] ^= state->s[3];

    state->s[2] ^= t;

    state->s[3] = rotl(state->s[3], 45);

    return result;
}

void* xoshiro256_init(u64 seed){
    xoshiro256_state* state=malloc(sizeof(xoshiro256_state));
    splitmix64_state* splitmix=splitmix64_init(seed);
    state->s[0]=splitmix64_next(splitmix);
    state->s[1]=splitmix64_next(splitmix);
    state->s[2]=splitmix64_next(splitmix);
    state->s[3]=splitmix64_next(splitmix);
    splitmix64_free(splitmix);
    return state;
}
