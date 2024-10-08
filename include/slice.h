#ifndef _SLICE_
#define _SLICE_

#include <stdlib.h>
#include <string.h>

typedef struct slice slice;

struct slice
{
    const char *_s;
    const char *_e;
};

const char* slice_to_cstr(slice* s);

#endif