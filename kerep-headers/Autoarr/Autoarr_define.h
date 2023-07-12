#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"

#define Autoarr_define(type, TYPE_IS_PTR) \
\
kt_define(Autoarr_##type, ____Autoarr_##type##_freeWithMembers, NULL); \
\
void __Autoarr_##type##_add(Autoarr_##type* ar, type element){ \
    if(!ar->values){ \
        ar->values=malloc(ar->max_blocks_count*sizeof(type*)); \
        goto create_block; \
    } \
    if(ar->block_length==ar->max_block_length){ \
        if (ar->blocks_count>=ar->max_blocks_count) throw(ERR_MAXLENGTH); \
        ar->block_length=0; \
create_block: \
        ar->values[ar->blocks_count]=malloc(ar->max_block_length*sizeof(type)); \
        ar->blocks_count++; \
    } \
    ar->values[ar->blocks_count-1][ar->block_length]=element; \
    ar->block_length++; \
} \
\
type __Autoarr_##type##_get(Autoarr_##type* ar, u32 index){ \
    if(index>=Autoarr_length(ar)) throw(ERR_WRONGINDEX); \
    return ar->values[index/ar->max_block_length][index%ar->max_block_length]; \
} \
\
type* __Autoarr_##type##_getPtr(Autoarr_##type* ar, u32 index){ \
    if(index>=Autoarr_length(ar)) throw(ERR_WRONGINDEX); \
    return ar->values[index/ar->max_block_length]+(index%ar->max_block_length); \
} \
\
void __Autoarr_##type##_set(Autoarr_##type* ar, u32 index, type element){ \
    if(index>=Autoarr_length(ar)) throw(ERR_WRONGINDEX); \
    ar->values[index/ar->max_block_length][index%ar->max_block_length]=element; \
} \
\
void __Autoarr_##type##_freeWithoutMembers(Autoarr_##type* ar, bool freePtr){ \
    for(u16 i=0; i<ar->blocks_count;i++) \
        free(ar->values[i]); \
    free(ar->values); \
    if(freePtr) free(ar); \
} \
\
void __Autoarr_##type##_freeWithMembers(Autoarr_##type* ar, bool freePtr){ \
    if(ktDescriptor_##type.freeMembers!=NULL) { \
        Autoarr_foreach(ar, el,  \
            void* members_ptr=&el; \
            if(TYPE_IS_PTR) members_ptr=*(type**)members_ptr; \
            ktDescriptor_##type.freeMembers(members_ptr); \
        ); \
    } \
    __Autoarr_##type##_freeWithoutMembers(ar, freePtr);\
} \
void ____Autoarr_##type##_freeWithMembers(void* ar){ \
    __Autoarr_##type##_freeWithMembers((Autoarr_##type*)ar, false); \
} \
\
type* __Autoarr_##type##_toArray(Autoarr_##type* ar){ \
    u32 length=Autoarr_length(ar); \
    if(length==0) \
        return NULL; \
    type* array=malloc(length * sizeof(type)); \
    for(u32 i=0; i<length; i++) \
        array[i]=__Autoarr_##type##_get(ar, i); \
    return array; \
} \
\
__Autoarr_##type##_functions_list_t __Autoarr_##type##_functions_list={ \
    &__Autoarr_##type##_add, \
    &__Autoarr_##type##_get, \
    &__Autoarr_##type##_getPtr, \
    &__Autoarr_##type##_set, \
    &__Autoarr_##type##_freeWithMembers, \
    &__Autoarr_##type##_freeWithoutMembers, \
    &__Autoarr_##type##_toArray \
}; \
\
Autoarr_##type* __Autoarr_##type##_create(u16 max_blocks_count, u16 max_block_length){ \
    Autoarr_##type* ar=malloc(sizeof(Autoarr_##type)); \
    *ar=(Autoarr_##type){ \
        .max_blocks_count=max_blocks_count, \
        .blocks_count=0, \
        .max_block_length=max_block_length, \
        .block_length=0, \
        .values=NULL, \
        .functions=&__Autoarr_##type##_functions_list \
    }; \
    return ar; \
}

#if __cplusplus
}
#endif