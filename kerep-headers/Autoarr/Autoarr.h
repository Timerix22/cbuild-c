#pragma once

#if __cplusplus
extern "C" {
#endif

#include "Autoarr_declare.h"
#include "Autoarr_define.h"

Autoarr_declare(Pointer)
Autoarr_declare(char)
Autoarr_declare(bool)
Autoarr_declare(f32)
Autoarr_declare(f64)
Autoarr_declare(i8)
Autoarr_declare(u8)
Autoarr_declare(i16)
Autoarr_declare(u16)
Autoarr_declare(i32)
Autoarr_declare(u32)
Autoarr_declare(i64)
Autoarr_declare(u64)

Autoarr_declare(Unitype)

#define Autoarr_foreach(ar, elem, codeblock...) { \
    if(ar->blocks_count>0) { \
        typeof(**ar->values) elem; \
        for(u16 blockI=0;blockI<ar->blocks_count-1;blockI++) \
            for(u32 elemI=0;elemI<ar->max_block_length;elemI++){ \
                elem=ar->values[blockI][elemI]; \
                { codeblock; } \
            } \
        for(u16 elemI=0;elemI<ar->block_length;elemI++){ \
            elem=ar->values[ar->blocks_count-1][elemI]; \
            { codeblock; } \
        } \
    } \
}

#if __cplusplus
}
#endif