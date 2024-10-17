#ifndef _UMAP_
#define _UMAP_

#include "help.h"
#include "slice.h"
#include <stdbool.h>
#include <string.h>

typedef struct umap umap;
typedef struct umap_entry umap_entry;

struct umap_entry
{
    char *key;
    uint64_t value; // can be converted to pointers too(obviously if the value was a valid pointer to begin with)!
    umap_entry *next;
};

// This is for strings as keys
struct umap
{
    umap_entry *buckets;
    size_t bucket_count;
    bool _e;
};

umap *umap_init(size_t bucket_count);

bool umap_insert(umap *map, char *key, uint64_t value);

bool umap_insert_slice(umap *map, slice *s, uint64_t value);

umap_entry *umap_find(umap *map, slice *s);

void umap_destroy(umap *map);

// We don't need to remove the entries until the compiler terminates.
// with umap_destroy, we will have to go through every bucket and free the 'next' entries

#endif