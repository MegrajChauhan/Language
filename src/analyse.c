#include "analyse.h"

bool analyse(namespace *ns, error *e)
{
    for (size_t i = 0; i < ns->nodes->count; i++)
    {
        node *n = (node *)vec_at(ns->nodes, i);
        switch (n->kind)
        {
        case VAR_DECLR:
        }
    }
}

bool variable_declaration(namespace *ns, node *n, error *e)
{
    /**
     * This alone is extremely complicated
     */
    // confirm that this variable is unique
    node_var_declr *vd = (node_var_declr *)n->_node;
    if (symtable_get(ns->table, vd->name))
    {
        // already declared before
        
    }
}