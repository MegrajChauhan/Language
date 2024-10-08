#ifndef _VEC_
#define _VEC_

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h> // for memcpy

#ifndef _VEC_FACTOR
#define _VEC_FACTOR 2 // increase the capacity by 2 times each time
#endif

typedef struct vec vec;

struct vec
{
    void *buf;    // the buffer
    size_t cap;   // capacity
    size_t count; // how much is used
    size_t elen;  // size of each element
};

vec *vec_init(size_t len, size_t elen);

bool vec_push(vec *v, void *elem);

bool vec_pop(vec *v);

void *vec_at(vec *v, size_t ind);

void vec_destroy(vec *v);

#endif
