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
    register size_t c_st = 0, o_st = 0;
    register char *i = l->iter;
    while (*i != 0)
    {
        if (IsSpace(*i))
        {
            while (IsSpace(*i))
                consume(l);
            i = l->iter;
        }
        else if (IsStrInclusive(*i))
        {
            group_characters(l, t);
            break;
        }
        else if (IsNum(*i))
        {
            if (!group_number(l, t))
                return false;
            break;
        }
        else if (*i == '/' && *(i + 1) == '/')
        {
            // a comment!
            while (*l->iter != 0 && *l->iter != '\n')
                consume(l);
            i = l->iter;
        }
        else if (*i == '@')
        {
            // we confirm the multi-line comments being valid during the first pass
            // there is hence no concern
            i++;
            while (*l->iter != 0 && *l->iter != '@')
                consume(l);
            l->iter++;
            i = l->iter;
        }
        else
        {
            // could be operator
            t->col = l->context->col;
            t->line = l->context->line;
            t->offset = l->context->offset;
            t->value._s = i;
            switch (*i)
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
            case ';':
                t->kind = SEMI_COLON;
                break;
            case '=':
            {
                switch (*(i + 1))
                {
                case '=':
                    t->kind = EQUALS;
                    i++;
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
            default:
            {
                // error
                error_add(l->err, l->context, CANNOT_BUILD_TOKEN, t->line, t->line, t->offset, t->offset + 1, t->col, t->col + 1);
                return false;
            }
            }
            t->value._e = (i++);
            l->iter = i;
            break;
        }
    }
    return true;
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
        l->context->offset++;
        l->iter++;
    }
    else
        l->context->col++;
    l->iter++;
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
            error_add(l->err, l->context, DOUBLE_DOTS_FLOAT, l->context->line, l->context->line, o_st, l->context->offset, c_st, l->context->col);
            return false;
        }
        consume(l);
    }
    // we have our number
    // even if the number ends with a '.'
    // it is acceptable
    t->value._e = (l->iter - 1);
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
    char *i = l->iter;
    while (*i != 0 && IsStrInclusive(*i))
        consume(l);
    t->value._e = (i - 1);
    if (!find_key(&t->value, &t->kind))
        t->kind = IDENTIFIER;
    t->col = c_st;
    t->line = l->context->line;
    t->offset = o_st;
    l->iter = i;
    return true;
}