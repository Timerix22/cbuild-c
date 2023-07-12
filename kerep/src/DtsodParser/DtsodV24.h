#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../Hashtable/Hashtable.h"

// parses text to binary values
Maybe DtsodV24_deserialize(char* text);

// creates text representation of dtsod
Maybe DtsodV24_serialize(Hashtable* dtsod);

#if __cplusplus
}
#endif