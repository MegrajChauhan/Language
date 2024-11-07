#ifndef _DEFS_
#define _DEFS_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// contains the necessary definitions only including functions and typedefs

typedef void (*__cleanup_func)(void *item);
typedef size_t (*_hash_func)(void *key, size_t bucket_count);
typedef bool (*_key_cmp_func)(void *key1, void *key2);

size_t string_hash(void *key, size_t bucket_count); // for hashing strings

#endif