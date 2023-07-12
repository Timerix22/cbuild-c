#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/type_system/base_toString.h"

// cross-platform printf analog
void kprintf(const char* format, ...);

#if __cplusplus
}
#endif