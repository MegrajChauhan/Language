#include "vec.h"

vec *vec_init(size_t len, size_t elen)
{
    vec *v = (vec *)malloc(sizeof(vec));
    if (!v)
        return NULL;

    v->buf = malloc(len * elen);
    if (!v->buf)
    {
        free(v);
        return NULL;
    }

    v->cap = len;
    v->count = 0;
    v->elen = elen;

    return v;
}

bool vec_push(vec *v, void *elem)
{
    if (v->count == v->cap)
    {
        size_t new_cap = v->cap * _VEC_FACTOR;
        void *new_buf = realloc(v->buf, new_cap * v->elen);
        if (!new_buf)
            return false;

        v->buf = new_buf;
        v->cap = new_cap;
    }

    memcpy((char *)v->buf + (v->count * v->elen), elem, v->elen);
    v->count++;
    return true;
}

bool vec_pop(vec *v)
{
    if (v->count == 0)
        return false;

    v->count--;
    return true;
}

void *vec_at(vec *v, size_t ind)
{
    if (ind >= v->count)
        return NULL;

    return (char *)v->buf + (ind * v->elen);
}

bool vec_crunch(vec *v)
{
    // crunch the vector to just fit the array
    // useful when the vector is sure to not grow
    // and this saves memory as well(though the chances of fragmentation increases)
    // i hope stdlib will move the array somewhere else to reduce fragmentation otherwise this will be useless to do
    v->buf = realloc(v->buf, v->count * v->elen);
    if (v->buf == NULL)
        return false;
    v->cap = v->count;
    return true;
}

void vec_destroy(vec *v)
{
    free(v->buf);
    free(v);
}