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
    p->error = false;
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
        error_add(p->err, &err, p->lex->context, t->kind == eof ? UNEXPECTED_EOF : UNEXPECTED_TOKEN, t->line, t->line, t->offset, t->offset + len, t->col, t->col + len);
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
        bool str = false;
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
        switch (tok.kind)
        {
        case SINGLE_QUOTE:
            lexer_next_token(p->lex, &tok);
            if (!lexer_request_character(p->lex, &tok))
                return false;
            tok.kind = STR;
            str = true;
            break;
        case QUOTE:
            lexer_next_token(p->lex, &tok);
            if (!lexer_request_string(p->lex, &tok))
                return false;
            tok.kind = STR;
            str = true;
            break;
        case PLUS:
        case MINUS:
        {
            // check if unary
            expression_nodes *prev = (expression_nodes *)vec_at(expr->nodes, expr->nodes->count - 1);
            uint64_t temp;
            if (prev->type == OPER && !(prev->tok_type >= OPEN_PAREN && prev->tok_type <= CLOSE_BIGBRAC))
            {
                if (find_oper(&prev->_var_or_value_or_oper_.name_or_value_or_oper, &temp) || expr->nodes->count == 0)
                    // so unary
                    tok.kind = tok.kind - 1;
            }
            break;
        }
        }
        expression_nodes n;
        n.tok_type = tok.kind;
        if (tok.kind >= COLON && tok.kind <= LESS_EQ)
            n.type = OPER;
        else if (tok.kind == STR || tok.kind == NUM_FLOAT || tok.kind == NUM_INT || tok.kind == FALSE || tok.kind == TRUE)
            n.type = DATA;
        else if (tok.kind == IDENTIFIER)
            n.type = VAR;
        else
        {
            error_unexp_tok err;
            err.got = tok.value;
            error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN_EXPR, tok.line, tok.line, tok.offset, tok.offset + (tok.value._e - tok.value._s), tok.col, tok.col + (tok.value._e - tok.value._s));
            return false;
        }
        n._var_or_value_or_oper_.name_or_value_or_oper = tok.value;
        n.offst = tok.offset;
        n.offed = tok.offset + (tok.value._e - tok.value._s);
        if (!vec_push(expr->nodes, &n))
        {
            internal_err();
            return false;
        }
        if (!str)
            lexer_next_token(p->lex, &tok);
    }
    if (expr->nodes->count == 0)
    {
        error_unexp_tok err;
        err.got = tok.value;
        error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN_EXPR, tok.line, tok.line, tok.offset, tok.offset + (tok.value._e - tok.value._s), tok.col, tok.col + (tok.value._e - tok.value._s));
        return false;
    }
    if (!vec_crunch(expr->nodes))
    {
        internal_err();
        return false;
    }
    return true;
}

bool parser_gen_type(parser *p, type *t, node *parent)
{
    // 't' will be allocated
    token tok;
    type *curr = t;
    if (!lexer_peek_token(p->lex, &tok))
        return false;
    if (tok.kind == ASSIGN)
    {
        error_unexp_tok err;
        err.exp = eof;
        err.got = tok.value;
        size_t len = tok.value._e - tok.value._s;
        error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN_TYPE, tok.line, tok.line, tok.offset, tok.offset + len, tok.col, tok.col + len);
        return false;
    }
    while (true)
    {
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
            curr->_length_ = 1;
            break;
        case U16:
            curr->base = UNSIGNED_WORD;
            curr->_length_ = 2;
            break;
        case U32:
            curr->base = UNSIGNED_DWORD;
            curr->_length_ = 4;
            break;
        case U64:
            curr->base = UNSIGNED_QWORD;
            curr->_length_ = 8;
            break;
        case I8:
            curr->base = SIGNED_BYTE;
            curr->_length_ = 1;
            break;
        case I16:
            curr->base = SIGNED_WORD;
            curr->_length_ = 2;
            break;
        case I32:
            curr->base = SIGNED_DWORD;
            curr->_length_ = 4;
            break;
        case I64:
            curr->base = SIGNED_QWORD;
            curr->_length_ = 8;
            break;
        case F32:
            curr->base = FLOAT;
            curr->_length_ = 4;
            break;
        case F64:
            curr->base = DOUBLE;
            curr->_length_ = 8;
            break;
        case BOOL:
            curr->base = BOOLEAN;
            curr->_length_ = 1;
            break;
        case CONST:
            curr->base = TCONST;
            curr->_length_ = 0;
            break;
        case VOID:
            curr->base = TVOID;
            curr->_length_ = 0;
            break;
        case IDENTIFIER:
            curr->base = USER_DEF;
            curr->_length_ = 0;
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
                lexer_next_token(p->lex, &_t);
                curr->expr.nodes = NULL;
                break;
            default:
                // by default, this becomes an expression
                if (!parse_add_expression(p, &curr->expr, CLOSE_BIGBRAC))
                    return false;
                curr->expr._type = ARR_LENGTH;
                curr->expr.parent = parent;
                // Array length evaluation is not compulsory to be known at
                // compile-time
            }
            curr->_length_ = 0; // gotta figure this out
            break;
        }
        default:
            // fatal error
            error_unexp_tok err;
            err.exp = eof;
            err.got = tok.value;
            size_t len = tok.value._e - tok.value._s;
            error_add(p->err, &err, p->lex->context, UNEXPECTED_TOKEN_TYPE, tok.line, tok.line, tok.offset, tok.offset + len, tok.col, tok.col + len);
            return false;
        }
        curr->off = tok.offset;
        curr->col = tok.col;
        curr->offe = p->lex->context->offset;
        if (!lexer_peek_token(p->lex, &tok))
            return false;
        if (tok.kind == ASSIGN)
        {
            curr->next = NULL;
            break;
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
    if (temp.kind == COLON)
    {
        // we have been given the types
        lexer_next_token(p->lex, &temp); // consume the colon
        t = malloc(sizeof(type));
        if (!t)
        {
            internal_err();
            goto _err_;
        }
        if (!parser_gen_type(p, t, n))
            goto _err_;
        if (!lexer_peek_token(p->lex, &temp))
            goto _err_;
    }
    if (!parser_check_token(p, &temp, ASSIGN))
        goto _err_;
    lexer_next_token(p->lex, &temp); // won't fail
    // the type deduction will be complicated but we need to do that at later steps
    if (!parse_add_expression(p, &vd->expr, SEMI_COLON))
        goto _err_;
    vd->expr._type = NORMAL_EXPR;
    vd->expr.parent = n;
    if (_const)
        vd->expr.must_eval = true;
    n->_node = (void *)vd;
    n->kind = VAR_DECLR;
    n->l_st = old_tok->line;
    n->c_st = old_tok->col;
    n->o_st = old_tok->offset;
    n->l_ed = temp.line;
    n->c_ed = temp.col;
    n->o_ed = temp.offset;
    vd->_t = t;
    vd->name = name.value;
    vd->_const = _const;
    if (!namespace_add_node(p->ns, n))
    {
        internal_err();
        goto _err_;
    }
    return true;
_err_:
    free(n);
    free(vd);
    return false;
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
            if (!parse_var_declr(p, false, &t))
                return false;
            break;
        case CONST:
            if (!parse_var_declr(p, true, &t))
                return false;
            break;
        }
    }
    return true;
}