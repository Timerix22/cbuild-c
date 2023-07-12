#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../std.h"
#include "ktid.h"
#include "typedef_macros.h"

#define kt_declare(TYPE)\
    ktid_declare(TYPE);\
    extern ktDescriptor ktDescriptor_##TYPE; \
    extern ktDescriptor ktDescriptor_##TYPE##_Ptr;

#define kt_define(TYPE, FREE_MEMBERS_F, TOSTRING_F)\
    ktid_define(TYPE); \
    ktDescriptor ktDescriptor_##TYPE={ \
        .name=#TYPE, \
        .id=ktid_undefined, \
        .size=sizeof(TYPE), \
        .freeMembers=FREE_MEMBERS_F, \
        .toString=TOSTRING_F \
    }; \
    ktDescriptor ktDescriptor_##TYPE##_Ptr={\
        .name=#TYPE "_Ptr", \
        .id=ktid_undefined, \
        .size=sizeof(TYPE), \
        .freeMembers=FREE_MEMBERS_F, \
        .toString=TOSTRING_F \
    };

typedef void (*freeMembers_t)(void*);
typedef char* (*toString_t)(void* obj, u32 fmt);

STRUCT(ktDescriptor,
    char* name;
    ktid id;
    u16 size;
    freeMembers_t freeMembers; // NULL or function which frees all struct members
    toString_t toString; // NULL or function which generates string representaion of object
)

/// gets descriptor for TYPE
#define ktDescriptor_name(TYPE) ktDescriptor_##TYPE
/// gets descriptor for pointer to TYPE
#define ktDescriptor_namePtr(TYPE) ktDescriptor_##TYPE##_Ptr

#if __cplusplus
}
#endif