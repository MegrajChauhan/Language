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

void error_add(error *e, file_context *fcont, uint64_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce)
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
    if (!vec_push(e->entries, (void *)(&ent)))
    {
        // error handler getting error itself is unacceptable
        // so we terminate violently here
        fprintf(stderr, "ERROR_HANDLER_INSANITY\n");
        exit(EXIT_FAILURE);
    }
}
void error_evaluate(error *e)
{
    // print the errors
}

void error_destroy(error *e)
{
    vec_destroy(e->entries);
    free(e);
}