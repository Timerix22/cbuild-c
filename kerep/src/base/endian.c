#include "endian.h"

static const union
{
    u16 number;
    Endian bytes[2];
} _endian_union={ .number=0x0102 };

Endian getEndian(){
    // if 0x0102 == { 1, 2 } then BigEndian
    // if 0x0102 == { 2, 1 } then LittleEndian
    return _endian_union.bytes[1];
}
