#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"

#define Autoarr_declare(type) \
\
struct Autoarr_##type; \
\
typedef struct __Autoarr_##type##_functions_list_t { \
    void (*add)(struct Autoarr_##type* ar, type element); \
    type (*get)(struct Autoarr_##type* ar, u32 index); \
    type* (*getPtr)(struct Autoarr_##type* ar, u32 index); \
    void (*set)(struct Autoarr_##type* ar, u32 index, type element); \
    void (*freeWithMembers)(struct Autoarr_##type* ar, bool freePtr); \
    void (*freeWithoutMembers)(struct Autoarr_##type* ar, bool freePtr); \
    type* (*toArray)(struct Autoarr_##type* ar); \
} __Autoarr_##type##_functions_list_t; \
\
extern __Autoarr_##type##_functions_list_t __Autoarr_##type##_functions_list; \
\
STRUCT(Autoarr_##type, \
    u16 blocks_count; \
    u16 max_blocks_count; \
    u16 block_length; \
    u16 max_block_length; \
    type** values; \
    __Autoarr_##type##_functions_list_t* functions; \
) \
\
Autoarr_##type* __Autoarr_##type##_create(u16 max_blocks_count, u16 max_block_length); \
void __Autoarr_##type##_freeWithMembers(Autoarr_##type* ar, bool freePtr); \
void ____Autoarr_##type##_freeWithMembers(void* ar);

#define Autoarr(type) Autoarr_##type

#define Autoarr_create(type, max_blocks_count, max_block_length) \
    __Autoarr_##type##_create(max_blocks_count, max_block_length)
#define Autoarr_add(autoarr, element) \
    autoarr->functions->add(autoarr, element)
#define Autoarr_get(autoarr, index) \
    autoarr->functions->get(autoarr,index)
#define Autoarr_getPtr(autoarr, index) \
    autoarr->functions->getPtr(autoarr,index)
#define Autoarr_set(autoarr, index, element) \
    autoarr->functions->set(autoarr, index, element)
#define Autoarr_free(autoarr, freePtr) \
    autoarr->functions->freeWithMembers(autoarr, freePtr)
#define Autoarr_freeWithoutMembers(autoarr, freePtr) \
    autoarr->functions->freeWithoutMembers(autoarr, freePtr)
#define Autoarr_toArray(autoarr) \
    autoarr->functions->toArray(autoarr)

#define Autoarr_length(autoarr) \
    (u32)(!autoarr->blocks_count ? 0 : \
    autoarr->max_block_length*(autoarr->blocks_count-1)+autoarr->block_length)
#define Autoarr_max_length(autoarr) \
    (u32)(autoarr->max_block_length*autoarr->max_blocks_count)

#define Autoarr_pop(AR){ \
    if(AR->block_length==1){ \
        AR->blocks_count--; \
        AR->block_length=AR->max_block_length; \
        free(AR->values[AR->blocks_count]); \
    } \
    else AR->block_length--; \
}

#if __cplusplus
}
#endif