#ifndef _HELP_
#define _HELP_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define IsSpace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define IsNum(c) ((c >= '0' && c <= '9'))
#define IsAlpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' &&  c <= 'Z'))
#define IsAlnum(c) (IsNum(c) || IsAlpha(c))
#define IsStrInclusive(c) (IsAlnum(c) || c == '_')

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