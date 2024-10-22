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
    case INVALID_EXPR:
        return sizeof(error_inval_expr);
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
        case INVALID_EXPR:
            fatal = true;
            __invalid_expr(ent);
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
        while (*i != '\n' && *i != 0)
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
    while (*i != 0 && *i != '\n' && *i != 0)
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
    while (*i != '\n' && *i != 0)
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
    while (*i != '\n' && *i != 0)
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
    char err_type[err->_the_node_->offe - err->_the_node_->off + 1];
    memcpy(err_type, e->error_context->entry.stream + err->_the_node_->off, sizeof(err_type) - 1);
    err_type[sizeof(err_type)] = 0;
    fprintf(stderr, "%s:%lu:%lu: Invalid type expression encountered. Expected a valid type in the expression. Unexpected '%s'.\n", e->error_context->entry.fname, err->n->l_st, err->_the_node_->col, err_type);
    fprintf(stderr, "LINE:\n\t");
    char *i = e->error_context->entry.stream + err->n->o_st;
    size_t expr_start = err->st->off - err->n->o_st;
    size_t node_pos = err->_the_node_->off - err->n->o_st;
    size_t node_pos_len = err->_the_node_->offe - err->n->o_st;
    type *t = err->_the_node_;
    while (t->next != NULL)
        t = t->next;
    size_t expr_end = t->offe - err->n->o_st;
    size_t line_len = err->n->o_ed - err->n->o_st;
    size_t X = 0;
    while (true)
    {
        size_t len = 0;
        while (*i != '\n' && *i != 0)
        {
            putc(*i, stderr);
            i++;
            len++;
        }
        putc('\n', stderr);
        putc('\t', stderr);
        for (size_t x = 0; x < len; X++, x++)
        {
            if ((X >= expr_start && X < node_pos) || (X >= node_pos_len && X < expr_end))
                putc('~', stderr);
            else if (X >= node_pos && X < node_pos_len)
                putc('^', stderr);
            else
                putc(' ', stderr);
        }
        if (X >= line_len)
            break;
        putc('\n', stderr);
        putc('\t', stderr);
        i++;
    }
    putc('\n', stderr);
}

void __redeclration(error_entry *e)
{
    error_redeclr *err = e->err;
    fprintf(stderr, "%s:%lu:%lu: Redeclaration of variable.\n", e->error_context->entry.fname, err->redec->l_st, err->redec->c_st);
    fprintf(stderr, "LINE:");
    char *i = e->error_context->entry.stream + (err->redec->o_st);
    for (size_t j = err->redec->l_st; j <= err->redec->l_ed; j++)
    {
        putc('\n', stderr);
        putc('\t', stderr);
        while (*i != '\n' && *i != 0)
        {
            putc(*i, stderr);
            i++;
        }
    }
    putc('\n', stderr);
    node *n = (node *)err->original->ptr;
    fprintf(stderr, "%s:%lu:%lu: Declared here first.\n", err->original->parent->entry.fname, n->l_st, n->c_st);
    fprintf(stderr, "LINE:");
    i = err->original->parent->entry.stream + (n->o_st);
    for (size_t j = n->l_st; j <= n->l_ed; j++)
    {
        putc('\n', stderr);
        putc('\t', stderr);
        while (*i != '\n' && *i != 0)
        {
            putc(*i, stderr);
            i++;
        }
    }
    putc('\n', stderr);
}

void __invalid_expr(error_entry *e)
{
    error_inval_expr *err = e->err;
    char err_node[err->err_off_ed - err->err_off_st + 1];
    memcpy(err_node, e->error_context->entry.stream + err->err_off_st, sizeof(err_node) - 1);
    err_node[sizeof(err_node)] = 0;
    node *parent = err->expr->parent;
    fprintf(stderr, "%s:%lu:%lu: Invalid expression found. Unexpected '%s' in the expression.\n", e->error_context->entry.fname, parent->l_st, parent->c_st, err_node);
    fprintf(stderr, "LINE:\n\t");
    char *i = e->error_context->entry.stream + (parent->o_st);
    size_t X = 0;
    expression_nodes *first = (expression_nodes *)vec_at(err->expr->nodes, 0);
    expression_nodes *last = (expression_nodes *)vec_at(err->expr->nodes, err->expr->nodes->count - 1);
    size_t node_pos = err->err_off_st - parent->o_st;
    size_t expr_start = first->offst - parent->o_st;
    size_t node_pos_len = err->err_off_ed - parent->o_st;
    size_t expr_end = last->offed - parent->o_st;
    size_t count = parent->l_st;
    while (true)
    {
        size_t len = 0;
        while (*i != '\n' && *i != 0)
        {
            putc(*i, stderr);
            i++;
            len++;
        }
        putc('\n', stderr);
        putc('\t', stderr);
        for (size_t x = 0; x < len; X++, x++)
        {
            if ((X >= expr_start && X < node_pos) || (X >= node_pos_len && X < expr_end))
                putc('~', stderr);
            else if (X >= node_pos && X < node_pos_len)
                putc('^', stderr);
            else
                putc(' ', stderr);
        }
        if (count == parent->l_ed)
            break;
        putc('\n', stderr);
        putc('\t', stderr);
        count++;
        i++;
    }
    putc('\n', stderr);
}