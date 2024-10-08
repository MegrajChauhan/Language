#include "uset.h"

uset *uset_init(size_t bucket_count)
{
    uset *set = (uset *)malloc(sizeof(uset));
    if (!set)
        return NULL;
    set->bucket_count = bucket_count;
    set->buckets = (uset_entry *)calloc(bucket_count, sizeof(uset_entry));
    if (set->_error)
    {
        free(set);
        return NULL;
    }
    for (size_t i = 0; i < bucket_count; i++)
    {
        set->buckets[i].key = NULL;
        set->buckets[i].next = NULL;
    }
    set->_error = false;
    return set;
}

bool uset_insert(uset *set, const char *key)
{
    size_t index = hash_string(key, set->bucket_count);
    uset_entry *entry = &set->buckets[index];

    if (entry->key == NULL)
    {
        entry->key = strdup(key);
        if (entry->key == NULL)
        {
            set->_error = true;
            return false;
        }
        return true;
    }

    uset_entry *current = entry;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
            return false; // Key already exists, do not insert
        if (current->next == NULL)
            break; // Reached the end of the linked list
        current = current->next;
    }

    // At this point, we need to allocate a new entry for the new key
    uset_entry *new_entry = (uset_entry *)malloc(sizeof(uset_entry));
    if (!new_entry)
    {
        set->_error = true;
        return false;
    }
    new_entry->key = strdup(key);
    if (new_entry->key == NULL)
    {
        free(new_entry);
        set->_error = true;
        return false;
    }
    new_entry->next = NULL;
    current->next = new_entry;
    return true;
}

bool uset_contains(uset *set, const char *key)
{
    size_t index = hash_string(key, set->bucket_count);
    uset_entry *entry = &set->buckets[index];

    while (entry != NULL)
    {
        if (entry->key != NULL && strcmp(entry->key, key) == 0)
            return true; // Key found
        entry = entry->next;
    }
    return false;
}

void uset_destroy(uset *set)
{
    for (size_t i = 0; i < set->bucket_count; i++)
    {
        if (set->buckets[i].key != NULL)
        {
            free(set->buckets[i].key);
            uset_entry *entry = set->buckets[i].next;
            while (entry != NULL)
            {
                uset_entry *temp = entry;
                entry = entry->next;
                free(temp->key);
                free(temp);
            }
        }
    }
    free(set->buckets);
    free(set);
}