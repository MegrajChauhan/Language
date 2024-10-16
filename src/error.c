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

void error_add(error *e, void *err, file_context *fcont, uint64_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce)
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
    ent.err = err;
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
    bool fatal = false;
    for (size_t i = 0; i < entries; i++)
    {
        error_entry *ent = (error_entry *)vec_at(e->entries, i);
        switch (ent->kind)
        {
        case CANNOT_BUILD_TOKEN:
            fatal = true;
            __cannot_built_token(ent);
            break;
        case DOUBLE_DOTS_FLOAT:
            fatal = true;
            __double_dots_float(ent);
            break;
        case UNEXPECTED_TOKEN_TYPE:
            fatal = true;
            __unexpected_token(ent, "u8, u16, u32, u64, i8, i16, i32, i64, f32, f64, [] or a user-defined type");
            break;
        case UNEXPECTED_EOF:
            fatal = true;
            __unexpected_token(ent, NULL);
            break;
        case UNEXPECTED_TOKEN:
            fatal = true;
            __unexpected_token(ent, tok_to_str(((error_unexp_tok *)ent->err)->exp));
            break;
        }
    }
    if (fatal)
        exit(EXIT_FAILURE);
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

void __unexpected_token(error_entry *e, char *exp)
{
    if (e->kind == UNEXPECTED_EOF)
    {
        fprintf(stderr, "%s:%lu:%lu: Unexpected EOF when expecting a token.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
        return;
    }
    error_unexp_tok *err = (error_unexp_tok *)e->err;
    fprintf(stderr, "%s:%lu:%lu: Unexpected '", e->error_context->entry.fname, e->err_line_st, e->col_st);
    for (size_t i = 0; i < (err->got._e - err->got._s); i++)
    {
        putc(*(err->got._s + i), stderr);
    }
    fprintf(stderr, "' when expected %s:\n", exp);
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