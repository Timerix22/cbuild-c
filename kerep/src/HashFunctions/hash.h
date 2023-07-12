#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../base/base.h"

#define hashb(FUNC, BUF, LEN) FUNC(0xFFFFFFFF, BUF, LEN)
#define hashs(FUNC, STR)      FUNC(0xFFFFFFFF, STR, cptr_length(STR))

u32 hash_sdbm32(u32 oldhash, void* buf, u32 len);
u32 hash_crc32(u32 oldhash, void* buf, u32 len);

#if __cplusplus
}
#endif