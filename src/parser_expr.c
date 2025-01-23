#include "parser_expr.h"

ast *parse_ast(vec *expr, parser_state *st)
{
    check_ptr(expr, buffer);
    token curr = *(token *)vec_at(expr, 0);
    file_context *current = get_current_file_context();
    fmt_log("PARSING EXPRESSION: In Line %lu.\n", curr.line);
    ast *tree = (ast *)malloc(sizeof(ast));
    if (!tree)
    {
        fmt_internal_error("Failed to parse the expression at line %lu[FAILED TO ALLOCATE MEMORY].\n", curr.line);
        log("Failed to parse expression\n");
        return NULL;
    }

    if (!(tree->root = produce_new_node()))
    {
        report_internal_error("Failed to parse expressions.");
        log("Failed to parse expression\n");
        free(tree);
        return NULL;
    }

    // start parsing now
    // tree->root->node_token = curr;

    fmt_log("PARSING EXPRESSION(%sSUCCESS%s)", BOLDGREEN, RESET);
    return tree;
}

size_t find_next_ast_node(vec *expr, parser_state *st)
{
    check_ptr(expr, buffer);

    // We ignore parenthesis and brackets
    token lowest;
    size_t ind = 0;
    for (size_t i = 0; i < expr->elem_count; i++)
    {
        token this = *(token *)vec_at(expr, i);
        token err_tok;
        size_t first_oparen = (size_t)-1, first_obrac = (size_t)-1, last_cparen = 0, last_cbrac = 0;
        switch (this.kind)
        {
        case TOK_PAREN_OPEN:
        case TOK_SQ_BRAC_OPEN:
        {
            i++;
            int64_t pcount = 0, bcount = 0;
            if (this.kind == TOK_PAREN_OPEN)
            {
                if (first_oparen == (size_t)-1)
                    first_oparen = i;
                pcount++;
            }
            else
            {
                if (first_obrac == (size_t)-1)
                    first_obrac = i;
                bcount++;
            }
            while (i < expr->elem_count && pcount != 0 && bcount != 0)
            {
                this = *(token *)vec_at(expr, i);
                switch (this.kind)
                {
                case TOK_PAREN_OPEN:
                    if (first_oparen == (size_t)-1)
                        first_oparen = i;
                    pcount++;
                    break;
                case TOK_PAREN_CLOSE:
                    pcount--;
                    last_cparen = i;
                    break;
                case TOK_SQ_BRAC_OPEN:
                    if (first_obrac == (size_t)-1)
                        first_obrac = i;
                    bcount++;
                    break;
                case TOK_SQ_BRAC_CLOSE:
                    bcount--;
                    last_cbrac = i;
                    break;
                }
                i++;
            }
            if (pcount > 0)
            {
                /**
                 * The 'base' of parser state needs to be sent pre-populated
                 */
                st->base.kind = __STRAY_OPENING_PARENTHESIS;
                st->hdlr = stray_expression_enclosers;
                err_tok = *(token *)vec_at(expr, first_oparen);
                st->base.col = err_tok.col;
                parser_state_populate_suggesstion(st, "This '(' is not terminated.");
            }
            else if (pcount < 0)
            {
                st->base.kind = __STRAY_CLOSING_PARENTHESIS;
                st->hdlr = stray_expression_enclosers;
                err_tok = *(token *)vec_at(expr, last_cparen);
                st->base.col = err_tok.col;
                parser_state_populate_suggesstion(st, "This ')' has no opening.");
            }
            if (bcount > 0)
            {
                st->base.kind = __STRAY_OPENING_BRAC;
                st->hdlr = stray_expression_enclosers;
                err_tok = *(token *)vec_at(expr, first_obrac);
                st->base.col = err_tok.col;
                parser_state_populate_suggesstion(st, "This '[' is not terminated.");
            }
            else if (bcount < 0)
            {
                st->base.kind = __STRAY_CLOSING_BRAC;
                st->hdlr = stray_expression_enclosers;
                err_tok = *(token *)vec_at(expr, last_cbrac);
                st->base.col = err_tok.col;
                parser_state_populate_suggesstion(st, "This ']' has no opening.");
            }
        }
        case TOK_PLUS:
        case TOK_MINUS:
        {
            // both '+' and '-' have the lowest possible precendence thus both have the same rule
            lowest = this;
            ind = i;
            goto defer;
        }
        
        {
        }
        }
    }
defer:
    return ind;
}

bool build_ast_from_root(ast *tree, vec *expr, parser_state *st)
{
    check_ptr(tree, root);

    // expr contains all of the tokens that make up of the expression
    // For now, we will be preparing to parse array access as well
    // The way we will do this is by using a stack.
}