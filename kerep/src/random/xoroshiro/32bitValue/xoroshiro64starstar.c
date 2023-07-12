/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include "xoroshiro64.h"

/* This is xoroshiro64** 1.0, our 32-bit all-purpose, rock-solid,
   small-state generator. It is extremely fast and it passes all tests we
   are aware of, but its state space is not large enough for any parallel
   application.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoroshiro64* is even faster.

   The state must be seeded so that it is not everywhere zero. */


static inline u32 rotl(const u32 x, i32 k) {
    return (x << k) | (x >> (32 - k));
}

u32 xoroshiro64starstar_next(void* _state) {
    xoroshiro64_state* state=_state;
    const u32 s0 = state->s[0];
    u32 s1 = state->s[1];
    const u32 result = rotl(s0 * 0x9E3779BB, 5) * 5;

    s1 ^= s0;
    state->s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
    state->s[1] = rotl(s1, 13); // c

    return result;
}
