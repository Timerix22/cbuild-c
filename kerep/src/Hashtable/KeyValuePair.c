#include "KeyValuePair.h"

kt_define(KVPair, __KVPair_free, NULL);

Autoarr_define(KVPair, false)

// proper way to clean a KVP
void KVPair_free(KVPair p){
    free(p.key);
    Unitype_free(p.value);
}
void __KVPair_free(void* p){ KVPair_free(*(KVPair*)p); }

void printkvp(KVPair p){
    kprintf("{\"%s\", ",p.key);
    printuni(p.value);
    kprintf("}");
}
