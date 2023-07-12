#pragma once

#if __cplusplus
extern "C" {
#endif

#include "std.h"
#include "type_system/typedef_macros.h"

PACKED_ENUM(Endian,
    UnknownEndian=0,
    LittleEndian=1,
    BigEndian=2
)

Endian getEndian();

#if __cplusplus
}
#endif
