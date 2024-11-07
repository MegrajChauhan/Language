#include "defs.h"

size_t string_hash(void *key, size_t bucket_count)
{
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t hash = 0;
    size_t len = strlen((const char*)key);

    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *)(key);
    const uint8_t *tail = (const uint8_t *)((const char*)key + nblocks * 4);

    uint32_t k;
    for (int i = 0; i < nblocks; i++)
    {
        k = blocks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        hash ^= k;
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = hash * m + n;
    }

    k = 0;
    switch (len & 3)
    {
    case 3:
        k ^= tail[2] << 16;
    case 2:
        k ^= tail[1] << 8;
    case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        hash ^= k;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash % bucket_count;
}