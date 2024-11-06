#include "uset.h"

static _uset_bucket *find_bucket(uset *set, void *key, size_t *bucket_index)
{
    *bucket_index = set->hash_func(key, set->bucket_count);
    _uset_bucket *bucket = &set->buckets[*bucket_index];

    while (bucket && bucket->key)
    {
        if (set->cmp_func(bucket->key, key))
        {
            return bucket;
        }
        bucket = bucket->nxt_bucket;
    }

    return NULL;
}

uset *uset_create(size_t bucket_count, _hash_func hash_func, _key_cmp_func cmp_func, __cleanup_func kcf)
{
    uset *set = (uset *)malloc(sizeof(uset));
    if (!set)
    {
        report_internal_error("Failed to initialize the uset.");
        return NULL;
    }

    set->buckets = (_uset_bucket *)calloc(bucket_count, sizeof(_uset_bucket));
    if (!set->buckets)
    {
        free(set);
        report_internal_error("Failed to initialize the uset buckets.");
        return NULL;
    }

    for (size_t i = 0; i < bucket_count; i++)
    {
        set->buckets[i].key = NULL;
    }
    set->bucket_count = bucket_count;
    set->hash_func = hash_func;
    set->cmp_func = cmp_func;
    set->key_clean_func = kcf;
    return set;
}

bool uset_insert(uset *set, void *key)
{
    check_ptr(set, buckets);

    size_t bucket_index;
    _uset_bucket *bucket = find_bucket(set, key, &bucket_index);

    if (bucket)
        return false;

    if (set->buckets[bucket_index].key)
    {
        _uset_bucket *new_bucket = (_uset_bucket *)malloc(sizeof(_uset_bucket));
        if (!new_bucket)
        {
            report_internal_error("Failed to allocate new bucket");
            return false;
        }
        new_bucket->key = key;
        new_bucket->nxt_bucket = NULL;
        set->buckets[bucket_index].nxt_bucket = new_bucket;
    }
    else
    {
        set->buckets[bucket_index].key = key;
        set->buckets[bucket_index].nxt_bucket = NULL;
    }
    return true;
}

bool uset_contains(uset *set, void *key)
{
    check_ptr(set, buckets);

    size_t bucket_index;
    _uset_bucket *bucket = find_bucket(set, key, &bucket_index);

    return bucket != NULL;
}

void uset_destroy(uset *set)
{
    check_ptr(set, buckets);

    for (size_t i = 0; i < set->bucket_count; i++)
    {
        _uset_bucket *bucket = set->buckets[i].nxt_bucket;

        while (bucket)
        {
            _uset_bucket *next = bucket->nxt_bucket;
            if (set->key_clean_func)
                set->key_clean_func(bucket->key);
            free(bucket);
            bucket = next;
        }
    }
    free(set->buckets);
    free(set);
}
