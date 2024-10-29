#include "type.h"

bool is_type_array(type *type_to_check)
{
    type *curr = type_to_check;
    while (curr->next != NULL)
        curr = curr->next;
    // check if we have an array here
    if (curr->base == ARRAY)
        return true;
    return false;
}

size_t get_dimension_of_array(type *array_type)
{
    type *curr = array_type;
    while (curr->base != ARRAY)
        curr = curr->next;
    size_t count = 0;
    while (curr->base == ARRAY)
    {
        count++;
        curr = curr->next;
    }
    return count;
}

bool deduce_expression_type(type *type_of_var, expr_t *res)
{
    // we have to deduce the type of the expression and then based on that
    // proceed to create an expression
    if (is_type_array(type_of_var))
    {
        size_t dimension = get_dimension_of_array(type_of_var);
        if (dimension > 1)
        {
            if (is_array_fit_for_string(type_of_var))
                *res = STR_MULTI_DIMENSION_ARR_ASSIGNMENT; // for now
        }
        else
        {
            if (is_array_fit_for_string(type_of_var))
                *res = STR_ARR_ASSIGNMENT;
            else
                *res = ARR_ASSIGNMENT;
        }
    }
    else
        *res = NORMAL_EXPR;
    return true;
}

// bool is_type_primitive(type *type_to_check)
// {
//     if (type_to_check->base >= SIGNED_BYTE && type_to_check->base <= BOOLEAN)
//         return true;
//     return false;
// }

bool is_array_fit_for_string(type *array_type)
{
    // 'u8[]' can accept a string as the assignment.
    // 'u8[][]...' can also accept strings as the assignment(each string will be given the length of the longest one)
    /// NOTE: 'u8*[]' doesn't accept strings for assignments
    /// NOTE: 'u8**[]' also cannot accept strings for assignments
    if (array_type->base == UNSIGNED_BYTE)
        return true;
    return false;
}

bool deduce_variable_type(expression *expr, type *to_build, namespace *ns, error *err)
{
    // This is rather complicated but with the help of the expression, we should be able to do it
    bool _float_found_ = false;
    for (size_t i = 0; i < expr->nodes->count; i++)
    {
        expression_nodes *curr_node = (expression_nodes *)vec_at(expr->nodes, i);
        switch (curr_node->type)
        {
        case DATA:
        {
            // if (curr_node->tok_type == NUM_INT)
            if (curr_node->tok_type == NUM_FLOAT)
                _float_found_ = true;
            break;
        }
        case SYM:
        {
            // we need to get the type of this variable
            symtable_entry *ent = namespace_query_symtable(ns, curr_node->_var_or_value_or_oper_.name_or_value_or_oper);
            if (!ent)
            {
                error_inval_expr e;
                e.err_off_st = curr_node->offst;
                e.err_off_ed = curr_node->offed;
                e.expr = expr;
                error_add_complex(err, &e, ns->cont, EXPR_VAR_DOESNT_EXIST);
                return false;
            }
            // this variable basically inherits the type of the sub-variable
            to_build->base = ent->t->base;
            to_build->col = ent->t->col;
            // to_build->
        }
        }
    }
}

bool compare_type_size_not_considered(type *t1, type *t2)
{
    if (t1 == t2)
        return true;
    type *curr1 = t1;
    type *curr2 = t2;
    while (true)
    {
        if (!curr1 && !curr2)
            break;
        if ((curr1->base == curr2->base) || (curr1->base >= SIGNED_BYTE && curr1->base <= BOOLEAN && curr2->base >= SIGNED_BYTE && curr2->base <= BOOLEAN))
        {
            if (curr1->base == ARRAY)
            {
                if (curr1->_length_ != curr2->_length_)
                    return false;
            }
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        else
            return false;
    }
    return true;
}