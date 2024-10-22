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

bool vec_crunch(vec *v);

void vec_destroy(vec *v);

void vec_subvec(vec *v, vec *res, size_t st_ind);

void vec_prevec(vec *v, vec *res, size_t upto);

size_t vec_index_of(vec *v, void *elem);

void vec_remove(vec *v, size_t ind_start, size_t upto, void *elem);

vec *vec_create_sub(vec *parent, size_t st, size_t ed);

#endif
