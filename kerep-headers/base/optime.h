#pragma once

#include "std.h"

#define __optime_print(opname, t) \
    char tnames[3][3]={"s\0","ms","us"}; \
    i32 tni=0; \
    if(t>1000000){ \
        t/=1000000; \
        tni=0; \
    } else if(t>1000){ \
        t/=1000; \
        tni=1; \
    } else tni=2; \
    kprintf("\e[93moperation \e[94m%s\e[93m lasted \e[94m%f \e[93m%s\n", \
                                    opname,              t,  tnames[tni]);

#ifdef CLOCK_REALTIME 
/// executes codeblock and prints execution time
/// u64 op_i is counter of the internal loop
/// uses non-standard high-precision clock
#define optime(opname, repeats, codeblock...) { \
    struct timespec start, stop; \
    clock_gettime(CLOCK_REALTIME, &start); \
    for(u64 op_i=0;op_i<(u64)repeats;op_i++) \
        { codeblock; } \
    clock_gettime(CLOCK_REALTIME, &stop); \
    f64 t=(f64)(stop.tv_sec-start.tv_sec)*1000000+(f64)(stop.tv_nsec-start.tv_nsec)/1000; \
    __optime_print(opname,t); \
}
#else 
/// uses standard low precision clock
#define optime(opname, repeats, codeblock...) { \
    clock_t start=clock(); \
    for(u64 op_i=0;op_i<(u64)repeats;op_i++) \
        { codeblock; } \
    clock_t stop=clock(); \
    f64 t=(f64)(stop-start)/CLOCKS_PER_SEC*1000000; \
    __optime_print(opname,t); \
}
#endif
