#include "umap.h"

umap *umap_init(size_t bucket_count)
{
    umap *map = (umap *)malloc(sizeof(umap));
    if (!map)
        return NULL;
    map->bucket_count = bucket_count;
    map->buckets = (umap_entry *)calloc(bucket_count, sizeof(umap_entry));
    if (!map->buckets)
    {
        free(map);
        return NULL;
    }
    for (size_t i = 0; i < bucket_count; i++)
    {
        map->buckets[i].key = NULL;
        map->buckets[i].next = NULL;
    }
    map->_e = false;
    return map;
}

bool umap_insert(umap *map, char *key, uint64_t value)
{
    size_t index = hash_string(key, map->bucket_count);

    // Check if the bucket is in use
    if (map->buckets[index].key == NULL)
    {
        // Not in use; create a new entry
        map->buckets[index].key = strdup(key); // Duplicate the key to store it
        if (map->buckets[index].key == NULL)
        {
            map->_e = true;
            return false;
        }
        map->buckets[index].value = value;
        return true;
    }

    // If the bucket is already in use, update the value (or handle collision if necessary)
    if (strcmp(map->buckets[index].key, key) == 0)
    {
        map->buckets[index].value = value; // Update value if key matches
        return true;
    }
    umap_entry *curr = map->buckets[index].next;
    umap_entry *prev = &map->buckets[index];
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
        {
            curr->value = value;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    // new entry
    umap_entry *nentry = malloc(sizeof(umap_entry));
    if (nentry == NULL)
    {
        map->_e = true;
        return false;
    }
    nentry->key = strdup(key);
    if (nentry->key == NULL)
    {
        free(nentry);
        map->_e = true;
        return false;
    }
    nentry->next = NULL;
    nentry->value = value;
    prev->next = nentry;
    return true;
}

// Find an entry by key
umap_entry *umap_find(umap *map, slice *s)
{
    const char *key = slice_to_cstr(s);
    if (!key)
    {
        map->_e = true;
        return NULL; // Handle memory allocation failure
    }
    size_t index = hash_string(key, map->bucket_count);
    // Check if the bucket is in use
    if (map->buckets[index].key != NULL && strcmp(map->buckets[index].key, key) == 0)
    {
        free(key);                   // Free allocated key
        return &map->buckets[index]; // Found the entry
    }
    free(key);   // Free allocated key
    return NULL; // Not found
}

// Destroy the unordered map and free memory
void umap_destroy(umap *map)
{
    for (size_t i = 0; i < map->bucket_count; i++)
    {
        if (map->buckets[i].key != NULL)
        {
            free(map->buckets[i].key); // Free the duplicated keys
            umap_entry *curr = map->buckets[i].next;
            while (curr != NULL)
            {
                umap_entry *tmp = curr->next;
                free(curr->key);
                free(curr);
                curr = tmp;
            }
        }
    }
    free(map->buckets); // Free the buckets
    free(map);          // Free the map itself
}
