#include "expr.h"

expr_result *evaluate_expression(ast *tree, namespace *ns, error *e)
{
    // based on the type of expression, we perfrom the evaluation differently
    switch (tree->kind)
    {
    case NORMAL_EXPR:
    case ARR_LENGTH:
    }
}

expr_result *simple_expression_evaluation(expr_state *state)
{
    ast_node *curr_node = state->tree->root;
    if (!curr_node)
    {
        // clearly a mistake and should not happen
        // It is unlikely that a NULL ast_node gets here
        return NULL;
    }

    // evaluate just the left side first
}

bool evaluate_node(ast_node *n, expr_state *state)
{
    if (n->left)
        if (!evaluate_node(n->left, state))
            return false;
    if (n->right)
        if (!evaluate_node(n->right, state))
            return false;
    switch(n->n->type)
    {
        case SYM:
        {
            // check if the variable exists
            // if it does, replace with the value
        }
    }
}