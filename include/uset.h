#ifndef _USET_
#define _USET_

#include "help.h"
#include "slice.h"
#include <stdbool.h>
#include <string.h>

typedef struct uset uset;
typedef struct uset_entry uset_entry;

struct uset_entry
{
    char *key;
    uset_entry *next;
};

struct uset
{
    uset_entry *buckets;
    size_t bucket_count;
    bool _error;
};

uset *uset_init(size_t bucket_count);

bool uset_insert(uset *set, const char *key);

bool uset_contains(uset *set, const char *key);

void uset_destroy(uset *set);

#endif