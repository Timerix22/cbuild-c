#pragma once

#if __cplusplus
extern "C" {
#endif

#include "../Hashtable/Hashtable.h"

// parses text to binary values
Maybe DtsodV24_deserialize(char* text);

// creates text representation of dtsod
Maybe DtsodV24_serialize(Hashtable* dtsod);

// returns value or UniNull if key not found
Unitype DtsodV24_get(Hashtable* dtsod, char* key);

// adds or sets value
void DtsodV24_addOrSet(Hashtable* dtsod, char* key, Unitype value);

// checks for dtsod contains value or dont
bool DtsodV24_contains(Hashtable* dtsod, char* key);

// replaces value with UniNull if key exists in dtsod
bool DtsodV24_remove(Hashtable* dtsod, char* key);

// frees memory including memory of elements (hashtables, autoarrs, etc.)
void DtsodV24_free(Hashtable* dtsod);

#if __cplusplus
}
#endif