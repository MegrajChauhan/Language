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

size_t get_err_len(uint64_t kind)
{
    switch (kind)
    {
    case UNEXPECTED_EOF:
    case UNEXPECTED_EOL:
    case UNEXPECTED_TOKEN:
    case UNEXPECTED_TOKEN_TYPE:
        return sizeof(error_unexp_tok);
    case REDECLARATION:
        return sizeof(error_redeclr);
    case INVALID_TYPE_EXPR:
        return sizeof(error_inval_type_expr);
    }
    return 0;
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
    if (err != NULL)
    {
        size_t len = get_err_len(kind);
        ent.err = malloc((len));
        if (!ent.err)
        {
            fprintf(stderr, "ERROR_HANDLER_INSANITY\n");
            exit(EXIT_FAILURE);
        }
        memcpy(ent.err, err, len);
    }
    if (!vec_push(e->entries, (void *)(&ent)))
    {
        // error handler getting error itself is unacceptable
        // so we terminate violently here
        fprintf(stderr, "ERROR_HANDLER_INSANITY\n");
        exit(EXIT_FAILURE);
    }
}

void error_add_complex(error *e, void *err, file_context *cont, uint64_t kind)
{
    error_add(e, err, cont, kind, 0, 0, 0, 0, 0, 0);
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
        case UNEXPECTED_EOL:
        case UNEXPECTED_EOF:
            fatal = true;
            __unexpected_token(ent, NULL);
            break;
        case UNEXPECTED_TOKEN:
            fatal = true;
            __unexpected_token(ent, tok_to_str(((error_unexp_tok *)ent->err)->exp));
            break;
        case INVALID_STRING:
            fatal = true;
            __inval_string(ent);
            break;
        case INVALID_TYPE_EXPR:
            fatal = true;
            __invalid_type_expr(ent);
            break;
        case REDECLARATION:
            fatal = true;
            __redeclration(ent);
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

void multi_line_err(error_entry *e)
{
    size_t num_of_lines = e->err_line_ed - e->err_line_st;
    char *i = e->error_context->entry.stream + (e->offset_st - e->col_st);
    for (size_t lno = 0; lno <= (num_of_lines - 1); lno++)
    {
        size_t len = 0;
        while (*i != '\n')
        {
            putc(*i, stderr);
            i++;
            len++;
        }
        putc('\n', stderr);
        putc('\t', stderr);
        for (size_t j = 0; j < e->col_st; j++)
            putc(' ', stderr);
        for (size_t j = e->col_st; j < len; j++)
            putc('^', stderr);
        putc('\n', stderr);
        putc('\t', stderr);
        e->col_st = 0;
        i++;
    }
    while (*i != 0 && *i != '\n')
    {
        putc(*i, stderr);
        i++;
    }
    putc('\n', stderr);
    putc('\t', stderr);
    for (size_t j = e->col_st; j < e->col_ed; j++)
        putc('^', stderr);
    putc('\n', stderr);
}

void err_line_print(error_entry *e)
{
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
    err_line_print(e);
}

void __unexpected_token(error_entry *e, char *exp)
{
    if (e->kind == UNEXPECTED_EOF)
    {
        fprintf(stderr, "%s:%lu:%lu: Unexpected EOF when expecting a valid token, character or identifier.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
        return;
    }
    error_unexp_tok *err = (error_unexp_tok *)e->err;
    if (e->kind == UNEXPECTED_EOL)
        fprintf(stderr, "%s:%lu:%lu: This EOL was not expected here.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
    else
    {
        fprintf(stderr, "%s:%lu:%lu: Unexpected '", e->error_context->entry.fname, e->err_line_st, e->col_st);
        for (size_t i = 0; i < (err->got._e - err->got._s); i++)
        {
            putc(*(err->got._s + i), stderr);
        }
        fprintf(stderr, "' when expected %s:\n", exp);
    }
    fprintf(stderr, "LINE:\n\t");
    err_line_print(e);
    free(err);
}

void __inval_string(error_entry *e)
{
    fprintf(stderr, "%s:%lu:%lu: Invalid string defined here.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
    fprintf(stderr, "LINE:\n\t");
    if (e->err_line_st != e->err_line_ed)
        multi_line_err(e);
    else
        err_line_print(e);
}

void __invalid_type_expr(error_entry *e)
{
    error_inval_type_expr *err = e->err;
    fprintf(stderr, "%s:%lu:%lu: Invalid type expression encountered.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
    fprintf(stderr, "LINE:\n\t");
    char *i = e->error_context->entry.stream + (err->st->off - err->st->col);
    size_t expr_start = err->st->off - err->st->col;
    size_t node_pos = err->_the_node_->off - err->st->off;
    size_t node_pos_len = err->_the_node_->cole - err->_the_node_->col;
    size_t X = 0;
    type *temp = err->_the_node_;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    while (true)
    {
        size_t len = 0;
        while (*i != '\n')
        {
            putc(*i, stderr);
            i++;
            len++;
        }
        putc('\n', stderr);
        putc('\t', stderr);
        for (size_t a = X; a < (X + len); a++)
        {
            if ((a > expr_start && a < node_pos) || (a > (node_pos + node_pos_len)))
                putc('~', stderr);
            else if (a > node_pos && a < (node_pos + node_pos_len))
                putc('^', stderr);
            else
                putc(' ', stderr);
        }
        putc('\n', stderr);
        putc('\t', stderr);
    }
}

void __redeclration(error_entry *e)
{
    error_redeclr *err = e->err;
    fprintf(stderr, "%s:%lu:%lu: Redeclaration of variable.\n", e->error_context->entry.fname, e->err_line_st, e->col_st);
    fprintf(stderr, "LINE:");
    char *i = e->error_context->entry.stream + (err->redec->o_st);
    for (size_t j = err->redec->l_st; j < err->redec->l_ed; j++)
    {
        putc('\n', stderr);
        putc('\t', stderr);
        while (*i != '\n')
        {
            putc(*i, stderr);
            i++;
        }
    }
    node *n = (node *)err->original->ptr;
    fprintf(stderr, "%s:%lu:%lu: Declared here first.\n", err->original->parent->entry.fname, n->l_st, n->c_st);
    fprintf(stderr, "LINE:");
    i = err->original->parent->entry.stream + (n->o_st);
    for (size_t j = n->l_st; j < n->l_ed; j++)
    {
        putc('\n', stderr);
        putc('\t', stderr);
        while (*i != '\n')
        {
            putc(*i, stderr);
            i++;
        }
    }
}