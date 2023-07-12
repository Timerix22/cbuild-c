/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoroshiro128.h"

/* This is xoroshiro128** 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-poi32 numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline u64 rotl(const u64 x, i32 k) {
    return (x << k) | (x >> (64 - k));
}

u64 xoroshiro128starstar_next(void* _state){    
    xoroshiro128_state* state=_state;
    const u64 s0 = state->s[0];
    u64 s1 = state->s[1];
    const u64 result = rotl(s0 * 5, 7) * 9;

    s1 ^= s0;
    state->s[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    state->s[1] = rotl(s1, 37); // c

    return result;
}
