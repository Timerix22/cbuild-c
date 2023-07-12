#include "splitmix64.h"

/* 
This is a fixed-increment version of Java 8's SplittableRandom generator
See http://dx.doi.org/10.1145/2714064.2660195 and
http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html
It is a very fast generator passing BigCrush, and it can be useful if
for some reason you absolutely want 64 bits of state; otherwise, we
rather suggest to use a xoroshiro128+ (for moderately parallel
computations) or xorshift1024* (for massively parallel computations)
generator. 
*/

// The state can be seeded with any (upto) 64 bit integer value.

void* splitmix64_init(u64 seed){
    splitmix64_state* state=malloc(sizeof(splitmix64_state));
    *state=seed;
    return state;
}

u64 splitmix64_next(void* _state) {
    splitmix64_state* state=_state;
    // increment the state variable
    *state += 0x9e3779b97f4a7c15;
    // copy the state to a working variable
    u64 z = *state;
    // xor the variable with the variable right bit shifted 30 then multiply by a constant
    z = (z ^ (z>>30)) * 0xbf58476d1ce4e5b9;
    // xor the variable with the variable right bit shifted 27 then multiply by a constant
    z = (z ^ (z>>27)) * 0x94d049bb133111eb;
    // return the variable xored with itself right bit shifted 31
    return z ^ (z>>31);
}
