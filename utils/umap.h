#ifndef _UMAP_
#define _UMAP_

#include "compiler.h"
#include "report.h"
#include "utils.h"
#include "defs.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct umap umap;
typedef struct _umap_bucket _umap_bucket;

struct _umap_bucket
{
    void *value;
    void *key;
    _umap_bucket *nxt_bucket;
};

struct umap
{
    _umap_bucket *buckets;
    size_t bucket_count;
    _hash_func hash_func;
    _key_cmp_func cmp_func;
    __cleanup_func key_clean_func; // if the key needs to be cleaned
    __cleanup_func value_clean_func; // if the value needs to be cleaned
};

umap *umap_create(size_t bucket_count, _hash_func hash_func, _key_cmp_func cmp_func, __cleanup_func kcf, __cleanup_func vcf);

bool umap_insert(umap *map, void *key, void *value);

void *umap_find(umap *map, void *key);

void umap_destroy(umap *map);

#endif