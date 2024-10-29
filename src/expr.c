#include "expr.h"

expr_result *evaluate_expression(ast *tree, namespace *ns, error *e, node *parent)
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
    switch (n->n->type)
    {
    case SYM:
    {
        // check if the variable exists
        // if it does, replace with the value
        symtable_entry *ent = namespace_query_symtable(state->ns, n->n->_var_or_value_or_oper_.name_or_value_or_oper);
        if (!ent)
        {
            error_inval_expr err;
            err.err_off_st = n->n->offst;
            err.err_off_ed = n->n->offed;
            error_add_complex(state->e, &err, state->ns->cont, EXPR_VAR_DOESNT_EXIST);
            return false;
        }
        if (!last_var_matches_this_type(state, ent))
            return false;
        // replace the value now
        
    }
    }
}

bool last_var_matches_this_type(expr_state *state, symtable_entry *ent)
{
    if (!state->last_var_entry)
    {
        state->last_var_entry = ent;
        return true;
    }
    if (state->last_var_entry == ent)
        return true;
    if (!compare_type_size_not_considered(state->last_var_entry, ent->t))
    {
        error_type_mismatch err;
        err.ent1 = state->last_var_entry;
        err.ent2 = ent;
        err.parent = state->parent;
        error_add_complex(state->e, &err, state->ns->cont, VAR_TYPE_MISMATCH);
        return false;
    }
    state->last_var_entry = ent;
    return true;
}

void replace_var_value(symtable_entry *ent, ast_node *n)
{
    switch(ent->kind)
    {
        case _VARIABLE:
        {
            n->n->type = DATA;
            // n->n->_var_or_value_or_oper_.name_or_value_or_oper
        }
    }
}