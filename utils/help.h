#ifndef _HELP_
#define _HELP_

#include <stdint.h>
#include <stddef.h>
#include <string.h>


// #if defined(_WIN32) || defined(_WIN64)
// #include <direct.h>  // For Windows _getcwd
// #define gcd _getcwd
// #else
// #include <unistd.h>  // For Unix/Linux getcwd
// #define gcd getcwd
// #endif

uint32_t murmurhash3(const char *key, size_t len, uint32_t seed);

size_t hash_string(const char *key, size_t bucket_count);

#endif