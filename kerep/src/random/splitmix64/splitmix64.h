#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../../base/base.h"

typedef u64 splitmix64_state;
typedef void* splitmix64_statePtr;

splitmix64_statePtr splitmix64_init(u64 seed);
static inline splitmix64_statePtr splitmix64_initFromTime(void) { return splitmix64_init(time(NULL)); }

u64 splitmix64_next(splitmix64_statePtr);
static inline void splitmix64_free(splitmix64_statePtr state) {
    free(state); 
}

#if __cplusplus
}
#endif