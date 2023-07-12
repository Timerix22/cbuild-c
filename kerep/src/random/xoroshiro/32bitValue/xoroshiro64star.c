/*  Written in 2016 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoroshiro64.h"

/*  
This is xoroshiro64* 1.0, our best and fastest 32-bit small-state
generator for 32-bit floating-poi32 numbers. We suggest to use its
upper bits for floating-poi32 generation, as it is slightly faster than
xoroshiro64**. It passes all tests we are aware of except for linearity
tests, as the lowest six bits have low linear complexity, so if low
linear complexity is not considered an issue (as it is usually the
case) it can be used to generate 32-bit outputs, too.

We suggest to use a sign test to extract a random Boolean value, and
right shifts to extract subsets of bits.

The state must be seeded so that it is not everywhere zero. 
*/

static inline u32 rotl(const u32 x, i32 k) {
    return (x << k) | (x >> (32 - k));
}

u32 xoroshiro64star_next(void* _state) {
    xoroshiro64_state* state=_state;
    const u32 s0 = state->s[0];
    u32 s1 = state->s[1];
    const u32 result = s0 * 0x9E3779BB;

    s1 ^= s0;
    state->s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
    state->s[1] = rotl(s1, 13); // c

    return result;
}

void* xoroshiro64_init(u64 seed){
    xoroshiro64_state* state=malloc(sizeof(xoroshiro64_state));
    splitmix64_state* splitmix=splitmix64_init(seed);
    state->merged=splitmix64_next(splitmix);
    splitmix64_free(splitmix);
    return state;
}
