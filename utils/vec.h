#ifndef _VEC_
#define _VEC_

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "report.h"
#include "utils.h"
#include "compiler.h"

typedef struct vec vec;

// take the element and perform cleanup
typedef void (*__cleanup_func)(void *elem);

// This function takes an element and dumps it
typedef __cleanup_func __dump_func;

struct vec
{
    void *buffer;
    size_t elem_count;
    size_t elem_len;
    size_t elem_cap;
};

#define vec_full(vec_) (vec_->elem_count >= vec_->elem_cap)
#define vec_empty(vec_) (vec_->elem_count == 0)
#define vec_start(vec_) vec_at(vec_, 0)
#define vec_end(vec_) vec_at(vec_, vec_->count - 1)

vec *vec_init(size_t elem_len, size_t init_cap);

// increase the size of the vector by factor times
vec *vec_resize(vec *orig, size_t factor);

// resize the vector such that elem_count == elem_cap
vec *vec_crunch(vec *orig);

bool vec_push(vec *v, void *elem);

void *vec_pop(vec *v);

// get the element at index position
void *vec_at(vec *v, size_t index);

// calculate the element's position from the start of the vector buffer
size_t vec_index_of(vec *v, void *elem);

void vec_destroy(vec *v);

void vec_clean(vec *v, __cleanup_func func);

// Debug function
void vec_dump(vec *v, __dump_func func);

#endif