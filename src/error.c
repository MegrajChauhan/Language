#include "error.h"

error *error_init()
{
    error *e = malloc(sizeof(error));
    if (e == NULL)
        return NULL;
    e->entries = vec_init(20, sizeof(error_entry));
    if (!e->entries)
    {
        free(e);
        return NULL;
    }
    return e;
}

bool error_add(error *e, file_context *fcont, uint64_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce)
{
    Assert(e == NULL);
    Assert(fcont == NULL);
    error_entry ent;
    ent.error_context = fcont;
    ent.kind = kind;
    ent.err_line_st = els;
    ent.err_line_ed = ele;
    ent.offset_st = os;
    ent.offset_ed = oe;
    ent.col_st = cs;
    ent.col_ed = ce;
    return vec_push(e->entries, (void *)(&ent));
}
// void error_evaluate(error* e);

void error_destroy(error *e)
{
    vec_destroy(e->entries);
    free(e);
}