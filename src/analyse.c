#include "analyse.h"

bool analyse(namespace *ns, error *e)
{
    bool ret = true;
    for (size_t i = 0; i < ns->nodes->count; i++)
    {
        void *p = vec_at(ns->nodes, i);
        node *n = (node *)(*(uint64_t *)p);
        switch (n->kind)
        {
        case VAR_DECLR:
            if (!variable_declaration(ns, n, e))
                ret = false;
            break;
        }
    }
    return ret;
}

bool variable_declaration(namespace *ns, node *n, error *e)
{
    /**
     * This alone is extremely complicated
     */
    // confirm that this variable is unique
    node_var_declr *vd = (node_var_declr *)n->_node;
    symtable_entry *entr;
    if ((entr = namespace_query_symtable(ns, vd->name)) != NULL)
    {
        // already declared before
        error_redeclr rd;
        rd.original = entr;
        rd.redec = n;
        error_add_complex(e, &rd, ns->cont, REDECLARATION);
        return false;
    }
    // It hasn't been declared before.
    // We need to validate the type first
    if (vd->_t)
        if (!analyse_type(ns, n, vd->_t, e))
            return false;

    // evaluate the assigned expression
    ast *test = ast_init();
    if (!test)
    {
        internal_err();
        return false;
    }
    if (!expr_to_ast(test, &vd->expr, e, ns->cont))
        return false;
    ast_destroy(test);
    // add to the symbol table
    // symtable_entry new_ent;
    // new_ent.kind = _VARIABLE;
    // new_ent.parent = ns->cont;
    // new_ent.ptr = n;
    // new_ent.t = vd->_t;
    // new_ent._const = vd->_const;
    // if (!symtable_add(ns->table, &new_ent, vd->name))
    //     return false;
    return true;
}

bool analyse_type(namespace *ns, node *n, type *t, error *e)
{
    type *curr = t;
    switch (curr->base)
    {
    case SIGNED_BYTE:
    case UNSIGNED_BYTE:
    case SIGNED_WORD:
    case UNSIGNED_WORD:
    case SIGNED_DWORD:
    case UNSIGNED_DWORD:
    case SIGNED_QWORD:
    case UNSIGNED_QWORD:
    case FLOAT:
    case DOUBLE:
    {
        if (curr->next)
        {
            switch (curr->next->base)
            {
            case ARRAY:
            {
                // This is an array and we need to evaluate the length expression
                curr = curr->next;
                // handling multi-dimensional arrays
                while (curr && curr->base == ARRAY)
                {
                    if (curr->expr.nodes)
                    {
                        // we need to deduce the length ourselves from the given array
                        ast *test = ast_init();
                        if (!test)
                        {
                            internal_err();
                            return false;
                        }
                        curr->expr._type = ARR_LENGTH;
                        if (!expr_to_ast(test, &curr->expr, e, ns->cont))
                            return false;
                        ast_destroy(test);
                    }
                    curr = curr->next;
                }
                break;
            }
            default:
                error_inval_type_expr err;
                err._the_node_ = curr->next;
                err.st = t;
                err.n = n;
                error_add_complex(e, &err, ns->cont, INVALID_TYPE_EXPR);
                return false;
            }
        }
        break;
    }
    default:
        error_inval_type_expr err;
        err._the_node_ = curr;
        err.st = t;
        err.n = n;
        error_add_complex(e, &err, ns->cont, INVALID_TYPE_EXPR);
        return false;
    }
    return true;
}