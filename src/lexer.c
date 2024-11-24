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
    l->curr = *(char *)stream_read(l->_f->fdata);
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
            return get_number(l, t);
        else if (IsAlpha(l->curr) || l->curr == '_')
            return get_identifier(l, t);
        else
        {
            slice tmp;
            tmp.st = (char*)stream_at(s);
            lexer_add_error(l, __INVALID_TOKEN, &tmp, __unknown_token);
            return false;
        }
    }
    return false;
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
    t->value.ed = (char *)stream_at(l->_f->fdata);
    return true;
}

bool handle_binary(lexer *l, token *t)
{
    check_source(l);
    t->col = l->col;
    t->line = l->line;
    t->value.st = (char *)stream_at(l->_f->fdata);
    while (stream_has_more(l->_f->fdata) && (l->curr == '0' || l->curr == '1'))
    {
        update_lexer(l);
    }
    t->kind = TOK_NUM_BINARY;
    t->value.ed = (char *)stream_at(l->_f->fdata);
    return true;
}

bool handle_hex(lexer *l, token *t)
{
    check_source(l);
    t->col = l->col;
    t->line = l->line;
    t->value.st = (char *)stream_at(l->_f->fdata);
    while (stream_has_more(l->_f->fdata) && (IsNum(l->curr) || ((l->curr >= 'A' && l->curr <= 'Z') || (l->curr >= 'a' && l->curr <= 'z'))))
    {
        if ((l->curr >= 'a' && l->curr <= 'z'))
            *(char *)stream_at(l->_f->fdata) = l->curr - 32;
        update_lexer(l);
    }
    t->kind = TOK_NUM_HEX;
    t->value.ed = (char *)stream_at(l->_f->fdata);
    return true;
}

bool handle_oct(lexer *l, token *t)
{
    check_source(l);
    t->col = l->col;
    t->line = l->line;
    t->value.st = (char *)stream_at(l->_f->fdata);
    while (stream_has_more(l->_f->fdata) && (IsNum(l->curr) && (l->curr != '8' || l->curr != '9')))
    {
        if ((l->curr >= 'a' && l->curr <= 'z'))
            *(char *)stream_at(l->_f->fdata) = l->curr - 32;
        update_lexer(l);
    }
    t->kind = TOK_NUM_OCTAL;
    t->value.ed = (char *)stream_at(l->_f->fdata);
    return true;
}

bool get_number(lexer *l, token *t)
{
    switch (identify_number_type(l))
    {
    case TOK_NUM_BINARY:
        return handle_binary(l, t);
    case TOK_NUM_DECIMAL:
        return handle_decimal(l, t);
    case TOK_NUM_HEX:
        return handle_hex(l, t);
    case TOK_NUM_OCTAL:
        return handle_oct(l, t);
    }
    return false;
}

bool get_identifier(lexer *l, token *t)
{
    check_source(l);

    // For an identifier, the rules are the same as in C
    t->col = l->col;
    t->line = l->line;
    t->value.st = (char *)stream_at(l->_f->fdata);
    while (stream_has_more(l->_f->fdata) && IsIdenNameInclusive(l->curr))
        update_lexer(l);
    t->value.ed = (char *)stream_at(l->_f->fdata);
    t->kind = is_a_key(&t->value);
    return true;
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
