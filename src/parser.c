#include "parser.h"

parser *parser_init(file_context *fc, error *e, namespace *ns)
{
    parser *p = (parser *)malloc(sizeof(parser));
    if (p == NULL)
        return NULL;
    if ((p->lex = lexer_init(fc, e)) == NULL)
    {
        free(p);
        return NULL;
    }
    p->err = e;
    p->ns = ns;
    p->ns->_global = false; // the main namespace(a child can be global though)
    return p;
}

void parser_destroy(parser *p)
{
    // destroying namespace involves destroying all the nodes, symtable entries and the children namespaces
    destroy_lexer(p->lex);
    free(p);
}

bool parser_check_token(parser *p, token *t, uint64_t exp)
{
    size_t len = t->value._e - t->value._s;
    if (t->kind != exp)
    {
        error_add(p->err, p->lex->context, UNEXPECTED_TOKEN, t->line, t->line, t->offset, t->offset + len, t->col, t->col + len);
        return false;
    }
    return true;
}

bool parser_gen_type(parser *p, type *t)
{
    // 't' will be allocated
    token tok;
    type *curr = t;
    while (true)
    {
        if (!lexer_peek_token(p->lex, &tok))
            return false;
        if (tok.kind == ASSIGN)
            break;                      // we are done
        lexer_next_token(p->lex, &tok); // should not fail
        switch (tok.kind)
        {
        case U8:
            curr->base = UNSIGNED_BYTE;
            break;
        case U16:
            curr->base = UNSIGNED_WORD;
            break;
        case U32:
            curr->base = UNSIGNED_DWORD;
            break;
        case U64:
            curr->base = UNSIGNED_QWORD;
            break;
        case I8:
            curr->base = SIGNED_BYTE;
            break;
        case I16:
            curr->base = SIGNED_WORD;
            break;
        case I32:
            curr->base = SIGNED_DWORD;
            break;
        case I64:
            curr->base = SIGNED_QWORD;
            break;
        case F32:
            curr->base = FLOAT;
            break;
        case F64:
            curr->base = DOUBLE;
            break;
        case CONST:
            curr->base = TCONST;
            break;
        case VOID:
            curr->base = VOID;
            break;
        case OPEN_BIGBRAC:
        {
            // could be an array
            // three possibilties:
            // there is a size of the array given,
            // there is an expression for the array len,
            // there is nothing
            token _t;
            if (!lexer_peek_token(p->lex, &_t))
                return false;
            switch(_t.kind)
            {
                case CLOSE_BIGBRAC:
                 // nothing, so 
            }
        }
        default:
        }
    }
    // to validate if the given type makes sense or not will be a daunting task
    // we leave this for a later step to perform
    // we will need extreme checks and analysis on each node so everything makes sense
    // since we need it anyway, we will leave this for later
}

bool parse_var_declr(parser *p, bool _const)
{
    token name, temp;
    bool _t = true;
    type *t;
    if (!lexer_next_token(p->lex, &name))
        return false;
    if (!parser_check_token(p, &name, IDENTIFIER))
        return false;
    // we have the name of the variable now
    if (!lexer_peek_token(p->lex, &temp))
        return false;
    if (temp.kind == COLON)
    {
        // we have been given the types
        lexer_next_token(p->lex, &temp); // consume the colon
    }
    else
        _t = false;
    // the type deduction will be complicated but we need to do that at later steps
    node *n = (node *)malloc(sizeof(node));
    if (!n)
        return false;
    return true;
}

bool parse(parser *p)
{
    token t;
    while (lexer_next_token(p->lex, &t))
    {
        if (t.kind == eof)
            break;
        switch (t.kind)
        {
        case VAR:
        }
    }
}