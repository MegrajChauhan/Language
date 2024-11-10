#ifndef _SLICE_
#define _SLICE_

#include "utils.h"
#include "defs.h"
#include "report.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef union slice slice;

// This structure will be useful for saving memory as we may just save slices of the file's content instead of copying again and again
// This also gives us the information such as start offset and end offset
union slice
{
    // which is to be used is not obvious here without using an enum tag
    // but we won't need that as the use depends of the context itself.
    struct
    {
        char *st;
        char *ed;
    };
    char *_str; // We have to do this
};

// we won't necessarily store allocated slices while compiling but just in case
slice *slice_create(char *st, char *ed);

size_t slice_len(slice *sl);

char *slice_to_str(slice *sl);

// true for same and equal(in the context of using _str)
bool slice_cmp(void *sl1, void *sl2);

void slice_destroy(void *sl);

size_t slice_hash(void *sl, size_t bucket_count);

size_t slice_hash_str(void *sl, size_t bucket_count);

#endif