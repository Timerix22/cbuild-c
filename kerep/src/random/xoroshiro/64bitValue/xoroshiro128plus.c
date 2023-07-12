/*  Written in 2016-2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoroshiro128.h"

/* This is xoroshiro128+ 1.0, our best and fastest small-state generator
   for floating-poi32 numbers, but its state space is large enough only
   for mild parallelism. We suggest to use its upper bits for
   floating-poi32 generation, as it is slightly faster than
   xoroshiro128++/xoroshiro128**. It passes all tests we are aware of
   except for the four lower bits, which might fail linearity tests (and
   just those), so if low linear complexity is not considered an issue (as
   it is usually the case) it can be used to generate 64-bit outputs, too;
   moreover, this generator has a very mild Hamming-weight dependency
   making our test (http://prng.di.unimi.it/hwd.php) fail after 5 TB of
   output; we believe this slight bias cannot affect any application. If
   you are concerned, use xoroshiro128++, xoroshiro128** or xoshiro256+.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. 

   NOTE: the parameters (a=24, b=16, b=37) of this version give slightly
   better results in our test than the 2016 version (a=55, b=14, c=36).
*/

static inline u64 rotl(const u64 x, i32 k) {
    return (x << k) | (x >> (64 - k));
}

u64 xoroshiro128plus_next(void* _state){    
    xoroshiro128_state* state=_state;
    const u64 s0 = state->s[0];
    u64 s1 = state->s[1];
    const u64 result = s0 + s1;

    s1 ^= s0;
    state->s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    state->s[1] = rotl(s1, 37); // c

    return result;
}

void* xoroshiro128_init(u64 seed){
    xoroshiro128_state* state=malloc(sizeof(xoroshiro128_state));
    splitmix64_state* splitmix=splitmix64_init(seed);
    state->s[0]=splitmix64_next(splitmix);
    state->s[1]=splitmix64_next(splitmix);
    splitmix64_free(splitmix);
    return state;
}
