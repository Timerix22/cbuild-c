#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"
#include "../Autoarr/Autoarr.h"

STRUCT(KVPair,
    char* key;
    Unitype value;
)

Autoarr_declare(KVPair)

// proper way to clean a KVP
void KVPair_free(KVPair p);
void __KVPair_free(void* p);

void printkvp(KVPair p);

#if __cplusplus
}
#endif