#include "umap.h"

static _umap_bucket *find_bucket(umap *map, void *key, size_t *bucket_index)
{
    *bucket_index = map->hash_func(key, map->bucket_count);
    _umap_bucket *bucket = &map->buckets[*bucket_index];

    while (bucket && bucket->key)
    {
        if (map->cmp_func(bucket->key, key))
        {
            return bucket;
        }
        bucket = bucket->nxt_bucket;
    }

    return NULL;
}

umap *umap_create(size_t bucket_count, _hash_func hash_func, _key_cmp_func cmp_func, __cleanup_func kcf, __cleanup_func vcf)
{
    umap *map = (umap *)malloc(sizeof(umap));
    if (!map)
    {
        report_internal_error("Failed to initialize the umap.");
        return NULL;
    }

    map->buckets = (_umap_bucket *)calloc(bucket_count, sizeof(_umap_bucket));
    if (!map->buckets)
    {
        free(map);
        report_internal_error("Failed to initialize the umap buckets.");
        return NULL;
    }
    for (size_t i = 0; i < bucket_count; i++)
    {
        map->buckets[i].key = NULL;
    }
    map->bucket_count = bucket_count;
    map->hash_func = hash_func;
    map->cmp_func = cmp_func;
    map->key_clean_func = kcf;
    map->value_clean_func = vcf;
    return map;
}

bool umap_insert(umap *map, void *key, void *value)
{
    check_ptr(map, buckets);

    size_t bucket_index;
    _umap_bucket *bucket = find_bucket(map, key, &bucket_index);

    if (bucket)
    {
        // key already exists, update value
        bucket->value = value;
        return true;
    }

    // insert new key-value pair
    if (map->buckets[bucket_index].key)
    {
        _umap_bucket *new_bucket = (_umap_bucket *)malloc(sizeof(_umap_bucket));
        if (!new_bucket)
        {
            report_internal_error("Failed to allocate new bucket");
            return false;
        }
        new_bucket->key = key;
        new_bucket->value = value;
        new_bucket->nxt_bucket = NULL;
        map->buckets[bucket_index].nxt_bucket = new_bucket;
    }
    else
    {
        map->buckets[bucket_index].key = key;
        map->buckets[bucket_index].value = value;
        map->buckets[bucket_index].nxt_bucket = NULL;
    }
    return true;
}

// Find Value by Key
void *umap_find(umap *map, void *key)
{
    check_ptr(map, buckets);

    size_t bucket_index;
    _umap_bucket *bucket = find_bucket(map, key, &bucket_index);

    return bucket ? bucket->value : NULL;
}

void umap_destroy(umap *map)
{
    check_ptr(map, buckets);

    for (size_t i = 0; i < map->bucket_count; i++)
    {
        _umap_bucket *bucket = map->buckets[i].nxt_bucket;

        while (bucket)
        {
            _umap_bucket *next = bucket->nxt_bucket;
            if (map->key_clean_func)
                map->key_clean_func(bucket->key);
            if (map->value_clean_func)
                map->value_clean_func(bucket->value);
            free(bucket);
            bucket = next;
        }
    }
    map->buckets = NULL;
    free(map->buckets);
    free(map);
}
