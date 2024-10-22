#include "lexer.h"

lexer *lexer_init(file_context *fc, error *e)
{
    lexer *l = (lexer *)malloc(sizeof(lexer));
    if (l == NULL)
        return NULL;
    l->context = fc;
    l->err = e;
    l->iter = fc->entry.stream;
    return l;
}

void destroy_lexer(lexer *l)
{
    free(l);
    // other fields are none of lexer's business
}

bool lexer_next_token(lexer *l, token *t)
{
    while (true)
    {
        if (*l->iter == 0)
        {
            t->kind = eof;
            break;
        }
        if (IsSpace(*l->iter))
        {
            while (IsSpace(*l->iter))
                consume(l);
        }
        else if (IsNum(*l->iter))
        {
            if (!group_number(l, t))
            {
                consume(l);
                return false;
            }
            break;
        }
        else if (IsStrInclusive(*l->iter))
        {
            group_characters(l, t);
            break;
        }
        else if (*l->iter == '/' && *(l->iter + 1) == '/')
        {
            // a comment!
            while (*l->iter != 0 && *l->iter != '\n')
                consume(l);
        }
        else if (*l->iter == '@')
        {
            // we confirm the multi-line comments being valid during the first pass
            // there is hence no concern
            consume(l);
            while (*l->iter != 0 && *l->iter != '@')
                consume(l);
            consume(l);
        }
        else
        {
            // could be operator
            t->col = l->context->col;
            t->line = l->context->line;
            t->offset = l->context->offset;
            t->value._s = l->iter;
            switch (*l->iter)
            {
            case '$':
                t->kind = INC_DOLLAR;
                break;
            case '#':
                t->kind = HASH_BLOCK;
                break;
            case ':':
                t->kind = COLON;
                break;
            case ',':
                t->kind = COMMA;
                break;
            case ';':
                t->kind = SEMI_COLON;
                break;
            case '=':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = EQUALS;
                    consume(l);
                    break;
                default:
                    t->kind = ASSIGN;
                }
                break;
            }
            case '(':
                t->kind = OPEN_PAREN;
                break;
            case ')':
                t->kind = CLOSE_PAREN;
                break;
            case '{':
                t->kind = OPEN_CURLY;
                break;
            case '}':
                t->kind = CLOSE_CURLY;
                break;
            case '[':
                t->kind = OPEN_BIGBRAC;
                break;
            case ']':
                t->kind = CLOSE_BIGBRAC;
                break;
            case '\"':
                t->kind = QUOTE;
                break;
            case '\'':
                t->kind = SINGLE_QUOTE;
                break;
            case '.':
                t->kind = ACCESS_DOT;
                break;
            case '+':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = PLUS_EQ;
                    consume(l);
                    break;
                case '+':
                    t->kind = INC;
                    consume(l);
                    break;
                default:
                    t->kind = PLUS;
                }
                break;
            }
            case '-':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = MINUS_EQ;
                    consume(l);
                    break;
                case '-':
                    t->kind = DEC;
                    consume(l);
                    break;
                default:
                    t->kind = MINUS;
                }
                break;
            }
            case '*':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = MUL_EQ;
                    consume(l);
                    break;
                default:
                    t->kind = MUL;
                }
                break;
            }
            case '/':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = DIV_EQ;
                    consume(l);
                    break;
                default:
                    t->kind = DIV;
                }
                break;
            }
            case '%':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = MOD_EQ;
                    consume(l);
                    break;
                default:
                    t->kind = MOD;
                }
                break;
            }
            case '!':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = NOT_EQUALS;
                    consume(l);
                    break;
                default:
                    t->kind = LNOT;
                }
                break;
            }
            case '|':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = OR_EQ;
                    consume(l);
                    break;
                case '|':
                    t->kind = LOR;
                    consume(l);
                    break;
                default:
                    t->kind = OR;
                }
                break;
            }
            case '&':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = AND_EQ;
                    consume(l);
                    break;
                case '|':
                    t->kind = LAND;
                    consume(l);
                    break;
                default:
                    t->kind = AND;
                }
                break;
            }
            case '~':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = NOT_EQ;
                    consume(l);
                    break;
                default:
                    t->kind = NOT;
                }
                break;
            }
            case '^':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = XOR_EQ;
                    consume(l);
                    break;
                default:
                    t->kind = XOR;
                }
                break;
            }
            case '>':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = GREATER_EQ;
                    consume(l);
                    break;
                case '>':
                    switch (*(l->iter + 2))
                    {
                    case '=':
                        t->kind = RSHIFT_EQ;
                        consume(l);
                        break;
                    default:
                        t->kind = RSHIFT;
                        consume(l);
                    }
                    break;
                default:
                    t->kind = GREATER_THAN;
                }
                break;
            }
            case '<':
            {
                switch (*(l->iter + 1))
                {
                case '=':
                    t->kind = LESS_EQ;
                    consume(l);
                    break;
                case '<':
                    switch (*(l->iter + 2))
                    {
                    case '=':
                        t->kind = LSHIFT_EQ;
                        consume(l);
                        break;
                    default:
                        t->kind = LSHIFT;
                        consume(l);
                    }
                    break;
                default:
                    t->kind = LESS_THAN;
                }
                break;
            }
            default:
            {
                // error
                error_add(l->err, NULL, l->context, CANNOT_BUILD_TOKEN, t->line, t->line, t->offset, t->offset + 1, t->col, t->col + 1);
                consume(l);
                return false;
            }
            }
            consume(l);
            t->value._e = l->iter;
            break;
        }
    }
    return true;
}

bool lexer_peek_token(lexer *l, token *t)
{
    size_t col = l->context->col;
    size_t line = l->context->line;
    size_t off = l->context->offset;
    char *i = l->iter;
    bool ret = lexer_next_token(l, t);
    l->context->col = col;
    l->context->line = line;
    l->context->offset = off;
    l->iter = i;
    return ret;
}

char peek(lexer *l)
{
    return (*l->iter == 0) ? 0 : *(l->iter + 1);
}

void consume(lexer *l)
{
    if (*l->iter == 0)
        return;
    if (*l->iter == '\n')
    {
        l->context->col = 0;
        l->context->line++;
    }
    else
        l->context->col++;
    l->iter++;
    l->context->offset++;
}

bool group_number(lexer *l, token *t)
{
    // read the numbers
    // this is called if we encounter a number so the start is already a number
    t->value._s = l->iter;
    size_t dot_count = 0;
    size_t c_st = l->context->col;
    size_t o_st = l->context->offset;
    while (*l->iter != 0 && (IsNum(*l->iter) || *l->iter == '.'))
    {
        if (*l->iter == '.')
            dot_count++;
        if (dot_count > 1)
        {
            error_add(l->err, NULL, l->context, DOUBLE_DOTS_FLOAT, l->context->line, l->context->line, o_st, l->context->offset, c_st, l->context->col);
            return false;
        }
        consume(l);
    }
    // we have our number
    // even if the number ends with a '.'
    // it is acceptable
    t->value._e = (l->iter);
    t->col = c_st;
    t->kind = (dot_count == 0) ? NUM_INT : NUM_FLOAT;
    t->line = l->context->line;
    t->offset = o_st;
    return true;
}

bool group_characters(lexer *l, token *t)
{
    // read the keywords or identifiers
    t->value._s = l->iter;
    size_t c_st = l->context->col;
    size_t o_st = l->context->offset;
    while (*l->iter != 0 && IsStrInclusive(*l->iter))
        consume(l);
    t->value._e = (l->iter);
    if (!find_key(&t->value, &t->kind))
        t->kind = IDENTIFIER;
    t->col = c_st;
    t->line = l->context->line;
    t->offset = o_st;
    return true;
}

bool lexer_request_character(lexer *l, token *t)
{
    // read just one character and return
    // if there is an ending quote is none of lexer's business
    // if there are escape sequences, we will handle them but not all of them just yet
    t->col = l->context->col;
    t->line = l->context->line;
    t->offset = l->context->offset;
    if (*l->iter == 0)
    {
        // unexpected EOF
        error_unexp_tok err;
        err.exp = 0; // we will handle this in the function
        err.got._s = l->iter - 1;
        err.got._e = l->iter;
        error_add(l->err, &err, l->context, UNEXPECTED_EOF, t->line, t->line, t->offset - 1, t->offset, t->col - 1, t->col);
        return false;
    }
    if (*l->iter == '\n')
    {
        error_unexp_tok err;
        err.exp = 0; // we will handle this in the function
        err.got._s = l->iter - 1;
        err.got._e = l->iter;
        error_add(l->err, &err, l->context, UNEXPECTED_EOL, t->line, t->line, t->offset - 1, t->offset, t->col - 1, t->col);
        return false;
    }
    t->value._s = l->iter;
    if (*l->iter == '\\')
    {
        consume(l);
        // the next character must be a character for now
        // the complex escape sequences won't be handled just yet
        switch (*l->iter)
        {
        case 'n':
        case 't':
        case 'r':
        case '\\':
        case '\"':
        case '\'':
            consume(l);
            break;
        default:
            error_add(l->err, NULL, l->context, INVALID_STRING, t->line, t->line, t->offset - 1, t->offset, t->col - 1, t->col);
            return false;
        }
    }
    consume(l);
    t->value._e = l->iter;
    if (*l->iter != '\'')
    {
        error_add(l->err, NULL, l->context, INVALID_STRING, t->line, t->line, t->offset - 1, l->context->offset, t->col - 1, l->context->col);
        return false;
    }
    t->kind = REQUEST;
    consume(l); // the single quote
    return true;
}

bool lexer_request_string(lexer *l, token *t)
{
    t->col = l->context->col;
    t->line = l->context->line;
    t->offset = l->context->offset;
    t->value._s = l->iter;
    while (*l->iter != '\"')
    {
        if (*l->iter == 0)
        {
            // unexpected EOF
            error_unexp_tok err;
            err.exp = 0; // we will handle this in the function
            err.got._s = l->iter - 1;
            err.got._e = l->iter;
            error_add(l->err, &err, l->context, INVALID_STRING, t->line, l->context->line, t->offset - 1, l->context->offset, t->col - 1, l->context->col);
            return false;
        }
        if (*l->iter == '\\')
        {
            consume(l);
            switch (*l->iter)
            {
            case 'n':
            case 't':
            case 'r':
            case '\\':
            case '\"':
            case '\'':
                consume(l);
                break;
            default:
                error_add(l->err, NULL, l->context, INVALID_STRING, t->line, l->context->line, t->offset - 1, l->context->offset, t->col - 1, l->context->col);
                return false;
            }
        }
        consume(l);
    }
    if (*l->iter != '\"')
    {
        error_add(l->err, NULL, l->context, INVALID_STRING, t->line, l->context->line, t->offset - 1, l->context->offset, t->col - 1, l->context->col);
        return false;
    }
    t->value._e = l->iter;
    t->kind = REQUEST;
    consume(l); // the quote
    return true;
}