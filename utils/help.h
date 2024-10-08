#ifndef _HELP_
#define _HELP_

#include <stdint.h>
#include <stddef.h>
#include <string.h>

uint32_t murmurhash3(const char *key, size_t len, uint32_t seed);

size_t hash_string(const char *key, size_t bucket_count);

#endif