#include "lexer.h"

lexer *lexer_init(file *_f)
{
    lexer *l = (lexer *)malloc(sizeof(lexer));
    if (!l)
    {
        report_internal_error("Failed to create a lexer.");
        return NULL;
    }
    l->col = 0;
    l->line = 1;
    l->_f = _f;
    return l;
}

void lexer_destroy(lexer *l)
{
    check_source(l);
    free(l);
}

void update_lexer(lexer *l)
{
    check_source(l);
    if (stream_has_more(l->_f->fdata))
    {
        if (l->curr == '\n')
        {
            l->col = 0;
            l->line++;
        }
        l->curr = *(char *)stream_read(l->_f->fdata);
    }
}

bool next_token(lexer *l, token *t)
{
    check_source(l);
    stream *s = l->_f->fdata;
    while (true)
    {
        if (!stream_has_more(s))
        {
            t->col = l->col;
            t->line = l->line;
            t->kind = TOK_EOF;
            return true;
        }
        if (IsSpace(l->curr))
        {
            while (stream_has_more(s) && IsSpace(l->curr))
                update_lexer(l);
        }
        else if (IsNum(l->curr))
        {
        }
    }
}

token_t identify_number_type(lexer *l)
{
    check_source(l);
    switch (l->curr)
    {
    case '0':
    {
        // could be anything
        char peek = *(char *)stream_peek(l->_f->fdata, 1);
        switch (peek)
        {
        case 'x':
            return TOK_NUM_HEX;
        case 'o':
            return TOK_NUM_OCTAL;
        case 'b':
            return TOK_NUM_BINARY;
        case 'd':
            return TOK_NUM_DECIMAL; // just to make things complete
        default:
        {
            slice tmp;
            tmp.st = (char *)stream_at(l->_f->fdata);
            lexer_add_error(l, __INVALID_NUMBER_BASE_TYPE, &tmp, __invalid_number_base_type);
            return TOK_ERROR;
        }
        }
    }
    default:
        return TOK_NUM_DECIMAL;
    }
    return TOK_ERROR;
}

bool handle_decimal(lexer *l, token *t)
{
    check_source(l);
    t->col = l->col;
    t->line = l->line;
    t->value.st = (char *)stream_at(l->_f->fdata);
    size_t dot_count = 0;
    while (stream_has_more(l->_f->fdata) && (IsNum(l->curr) || l->curr == '.'))
    {
        if (l->curr == '.')
            dot_count++;
        if (dot_count > 1)
        {
            lexer_add_error(l, __INVALID_FLOATING_POINT_NUMBER, &t->value, __invalid_floating_point_number);
            return false;
        }
        update_lexer(l);
    }
    if (dot_count > 0)
        t->kind = TOK_NUM_FLOAT;
    else
        t->kind = TOK_NUM_DECIMAL;
    t->value = 
    return true;
}

bool get_number(lexer *l, token *t)
{
}

void lexer_add_error(lexer *l, error_t type, slice *value, __error_hdlr hdlr)
{
    check_source(l);
    lexer_state *state = (lexer_state *)malloc(sizeof(lexer_state));
    if (!state)
    {
        // this is fatal as error handling itself cannot and should not fail
        report_internal_error("Failed to push error state(lexer)");
        crash();
    }
    state->col = l->col; // this is the point where the error occured
    state->line = l->line;
    state->val = *value;
    state->kind = type;
    add_error(state, l, hdlr);
}
