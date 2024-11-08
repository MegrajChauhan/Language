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
        }else if (IsNum(l->curr))
        {
            
        }
    }
}