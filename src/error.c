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
    Assert(e != NULL);
    Assert(fcont != NULL);
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
    // formatting the error could be done later
    size_t entries = e->entries->count;
    for (size_t i = 0; i < entries; i++)
    {
        error_entry *ent = (error_entry *)vec_at(e->entries, i);
        switch (ent->kind)
        {
        case CANNOT_BUILD_TOKEN:
            __cannot_built_token(ent);
            break;
        case DOUBLE_DOTS_FLOAT:
            __double_dots_float(ent);
            break;
        }
    }
}

void error_destroy(error *e)
{
    vec_destroy(e->entries);
    free(e);
}

void __cannot_built_token(error_entry *e)
{
    // while lexing
    fprintf(stderr, "%s:%lu:%lu: Cannot build a token from this. Unexpected '%c'.\n", e->error_context->entry.fname, e->err_line_st, e->col_st, *(e->error_context->entry.stream + e->offset_st));
    fprintf(stderr, "LINE:\n\t");
    char *i = e->error_context->entry.stream + (e->offset_st - e->col_st);
    while (*i != '\n')
    {
        putc(*i, stderr);
        i++;
    }
    putc('\n', stderr);
    putc('\t', stderr);
    for (size_t i = 0; i < e->col_st; i++)
    {
        putc(' ', stderr);
    }
    putc('^', stderr);
    putc('\n', stderr);
}

void __double_dots_float(error_entry *e)
{
    fprintf(stderr, "%s:%lu:%lu: Cannot build a number from this. Multiple '.' in a float is not allowed.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
    fprintf(stderr, "LINE:\n\t");
    char *i = e->error_context->entry.stream + (e->offset_st - e->col_st);
    while (*i != '\n')
    {
        putc(*i, stderr);
        i++;
    }
    putc('\n', stderr);
    putc('\t', stderr);
    for (size_t i = 0; i < e->col_st; i++)
    {
        putc(' ', stderr);
    }
    for (size_t i = e->col_st; i < e->col_ed; i++)
        putc('^', stderr);
    putc('\n', stderr);
}