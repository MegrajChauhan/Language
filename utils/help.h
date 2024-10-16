#ifndef _HELP_
#define _HELP_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "slice.h"
#include "defs.h"

#define IsSpace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define IsNum(c) ((c >= '0' && c <= '9'))
#define IsAlpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IsAlnum(c) (IsNum(c) || IsAlpha(c))
#define IsStrInclusive(c) (IsAlnum(c) || c == '_')

#if defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define SEPARATOR '/'
#else
#define SEPARATOR '\\'
#endif

#define internal_err() fprintf(stderr, "INTERNAL ERROR\n")

// #if defined(_WIN32) || defined(_WIN64)
// #include <direct.h>  // For Windows _getcwd
// #define gcd _getcwd
// #else
// #include <unistd.h>  // For Unix/Linux getcwd
// #define gcd getcwd
// #endif

uint32_t murmurhash3(const char *key, size_t len, uint32_t seed);

size_t hash_string(const char *key, size_t bucket_count);

bool get_filename_from_path(slice *path);

char *tok_to_str(uint64_t tok);

#endif