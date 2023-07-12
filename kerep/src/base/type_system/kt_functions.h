#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../std.h"
#include "ktid.h"
#include "ktDescriptor.h"

extern ktid ktid_last;
void __kt_register(ktDescriptor* descriptor);

#define kt_register(TYPE) \
    __kt_register(&ktDescriptor_##TYPE); \
    ktid_##TYPE=ktid_last; \
    __kt_register(&ktDescriptor_##TYPE##_Ptr); \
    ktid_##TYPE##_Ptr=ktid_last;

void kt_beginInit();
void kt_endInit();

/// @param id id of registered type
ktDescriptor* ktDescriptor_get(ktid id);

char* ktDescriptor_toString(ktDescriptor* d);

// call it to free heap-allocated ktDescriptors array
void kt_free();

kt_declare(Pointer);
kt_declare(char);
kt_declare(bool);
kt_declare(f32);
kt_declare(f64);
kt_declare(i8);
kt_declare(u8);
kt_declare(i16);
kt_declare(u16);
kt_declare(i32);
kt_declare(u32);
kt_declare(i64);
kt_declare(u64);

kt_declare(ktDescriptor);

#if __cplusplus
}
#endif