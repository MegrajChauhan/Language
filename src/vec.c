#include "vec.h"

vec *vec_init(size_t elem_len, size_t init_cap)
{
    vec *v = (vec *)malloc(sizeof(vec));
    if (!v)
    {
        report_internal_error("Failed to initialize a vector\n");
        return NULL;
    }
    v->buffer = (void *)malloc(init_cap * elem_len);
    if (!v->buffer)
    {
        report_internal_error("Failed to initialize vector buffer\n");
        free(v);
        return NULL;
    }
    v->elem_cap = init_cap;
    v->elem_count = 0;
    v->elem_len = elem_len;
    return v;
}

vec *vec_resize(vec *orig, size_t factor)
{
    /// NOTE: For those wondering, this check is for validating if the compiler itself corrupts the ptr somehow
    check_ptr(orig, buffer);
    orig->buffer = (void *)realloc(orig->buffer, orig->elem_cap * orig->elem_len * factor);
    if (!orig->buffer)
    {
        report_internal_error("Failed to resize vector buffer\n");
        return NULL;
    }
    orig->elem_cap *= factor;
    return orig;
}

vec *vec_crunch(vec *orig)
{
    check_ptr(orig, buffer);
    orig->buffer = (void *)realloc(orig->buffer, orig->elem_len * orig->elem_count);
    if (!orig->buffer)
    {
        report_internal_error("Failed to crunch vector buffer\n");
        return NULL;
    }
    orig->elem_cap = orig->elem_count;
    return orig;
}

bool vec_push(vec *v, void *elem)
{
    check_ptr(v, buffer);
    if (vec_full(v))
    {
        // try to resize the vector
        v = vec_resize(v, VEC_RESIZE_FACTOR);
        if (!v)
            return false;
    }
    void *current = (char *)v->buffer + v->elem_count * v->elem_len;
    memcpy(current, elem, v->elem_len); // shouldn't fail
    v->elem_count++;
    return true;
}

void *vec_pop(vec *v)
{
    // we will return a pointer to the popped element.
    // if it is used then good otherwise it maybe ignored as well!
    check_ptr(v, buffer);
    if (vec_empty(v))
    {
        fmt_internal_error("Failed to pop an element from a vector.\n", NULL);
        crash();
        return NULL;
    }
    void *popped_element = (char *)v->buffer + v->elem_len * v->elem_count;
    v->elem_count--;
    return popped_element;
}

void *vec_at(vec *v, size_t index)
{
    check_ptr(v, buffer);
    if (vec_empty(v))
    {
        // If index is supposed to be valid then why is the vector empty?
        fmt_internal_error("Failed to get the element at index of a vector.\n", NULL);
        crash();
        return NULL;
    }
    if (v->elem_count <= index)
    {
        fmt_internal_error("Invalid indexing for a vector.\n", NULL);
        crash();
        return NULL;
    }
    return ((char *)v->buffer + v->elem_len * index);
}

size_t vec_index_of(vec *v, void *elem)
{
    check_ptr(v, buffer);
    if (vec_empty(v))
    {
        // If elem is supposed to be valid then why is the vector empty?
        fmt_internal_error("Failed to get the index of element of a vector.\n", NULL);
        crash();
        return -1;
    }
    return ((elem - v->buffer) / v->elem_len);
}

// this becomes useful when the vector is holding pointers to some structure
void vec_clean(vec *v, __cleanup_func func)
{
    check_ptr(v, buffer);
    for (size_t i = 0; i < v->elem_count; i++)
        func(vec_at(v, i)); // the vec_at shouldn't fail here(it mustn't)
}

void vec_dump(vec *v, __dump_func func)
{
    vec_clean(v, func); // the same thing
}

void vec_destroy(vec *v)
{
    check_ptr(v, buffer);
    free(v->buffer);
    free(v);
    return;
}