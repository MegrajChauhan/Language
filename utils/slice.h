#ifndef _SLICE_
#define _SLICE_

#include "utils.h"
#include "report.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct slice slice;

// This structure will be useful for saving memory as we may just save slices of the file's content instead of copying again and again
// This also gives us the information such as start offset and end offset 
struct slice
{
    char *st;
    char *ed;
};

// we won't necessarily store allocated slices while compiling but just in case
slice *slice_create(char *st, char *ed);

size_t slice_len(slice *sl);

char *slice_to_str(slice *sl);

// true for same and equal
bool slice_cmp(slice *sl1, slice *sl2);

void slice_destroy(slice *sl);

#endif