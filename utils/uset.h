#ifndef _USET_
#define _USET_

#include "compiler.h"
#include "report.h"
#include "utils.h"
#include "defs.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct uset uset;
typedef struct _uset_bucket _uset_bucket;

struct _uset_bucket
{
    void *key;
    _uset_bucket *nxt_bucket;
};

struct uset
{
    _uset_bucket *buckets;
    size_t bucket_count;
    _hash_func hash_func;
    _key_cmp_func cmp_func;
    __cleanup_func key_clean_func; // cleanup function for the key
};

uset *uset_create(size_t bucket_count, _hash_func hash_func, _key_cmp_func cmp_func, __cleanup_func kcf);

bool uset_insert(uset *set, void *key);

bool uset_contains(uset *set, void *key);

void uset_destroy(uset *set);

#endif
