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
    p->_stat = false;
    p->err = false;
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
        error_unexp_tok err;
        err.exp = exp;
        err.got = t->value;
        error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN, t->line, t->line, t->offset, t->offset + len, t->col, t->col + len);
        return false;
    }
    return true;
}

bool parse_add_expression(parser *p, expression *expr, uint64_t until)
{
    token tok;
    expr->nodes = vec_init(5, sizeof(expression_nodes));
    if (!expr->nodes)
    {
        internal_err();
        return false;
    }
    while (true)
    {
        if (!lexer_peek_token(p->lex, &tok))
            return false;
        if (tok.kind == until)
        {
            lexer_next_token(p->lex, &tok);
            break;
        }
        if (tok.kind == eof)
        {
            // this is fatal
            return parser_check_token(p, &tok, until);
        }
        /// NOTE: We are not checking of the token is valid for an expression here.
        /// That extra check can be done with the expression evaluator which eases things
        expression_nodes n;
        n.type = tok.kind;
        n.val._s = tok.value._s;
        n.val._e = tok.value._e;
        if (!vec_push(expr->nodes, &n))
        {
            internal_err();
            return false;
        }
        lexer_next_token(p->lex, &tok);
    }
    if (!vec_crunch(expr->nodes))
    {
        internal_err();
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
            break; // we are done
        if (tok.kind == eof)
        {
            error_add(p->err, NULL, p->lex->context, UNEXPECTED_EOF, tok.line, tok.line, tok.offset, tok.offset + 1, tok.col, tok.col + 1);
            return false;
        }
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
            curr->base = ARRAY;
            switch (_t.kind)
            {
            case CLOSE_BIGBRAC:
                // nothing, so we need to figure out the length ourselves
                break;
            default:
                // by default, this becomes an expression
                if (!parse_add_expression(p, &curr->expr, CLOSE_BIGBRAC))
                    return false;
                // Array length evaluation is not compulsory to be known at
                // compile-time
            }
            break;
        }
        default:
            // fatal error
            error_unexp_tok err;
            err.exp = eof;
            err.got = tok.value;
            size_t len = tok.value._e - tok.value._s;
            error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN_ARR, tok.line, tok.line, tok.offset, tok.offset + len, tok.col, tok.col + len);
            return false;
        }
        curr->next = (type *)malloc(sizeof(type));
        if (!curr->next)
        {
            internal_err();
            return false;
        }
        curr = curr->next;
    }
    // to validate if the given type makes sense or not will be a daunting task
    // we leave this for a later step to perform
    // we will need extreme checks and analysis on each node so everything makes sense
    // since we need it anyway, we will leave this for later
    return true;
}

bool parse_var_declr(parser *p, bool _const, token *old_tok)
{
    token name, temp;
    type *t = NULL;
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
        t = malloc(sizeof(type));
        if (!t)
        {
            internal_err();
            return false;
        }
        if (!parser_gen_type(p, t))
            return false;
    }
    // GET THE EXPRESSION NOW
    // the type deduction will be complicated but we need to do that at later steps
    node *n = (node *)malloc(sizeof(node));
    node_var_declr *vd = (node_var_declr *)malloc(sizeof(node_var_declr));
    if (!n)
    {
        internal_err();
        return false;
    }
    if (!vd)
    {
        free(n);
        internal_err();
        return false;
    }
    n->_node = (void *)vd;
    n->kind = VAR_DECLR;
    n->l_st = old_tok->line;
    n->c_st = old_tok->col;
    n->o_st = old_tok->offset;
    vd->_t = t;
    vd->name = name.value;
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