#include "namespace.h"

namespace *namespace_init(namespace *parent)
{
    namespace *ns = (namespace *)malloc(sizeof(namespace));
    if (!ns)
        return NULL;
    ns->children = vec_init(20, sizeof(namespace *));
    if (!ns->children)
    {
        free(ns);
        return NULL;
    }
    ns->nodes = vec_init(20, sizeof(node *));
    if (!ns->nodes)
    {
        free(ns);
        vec_destroy(ns->children);
        return NULL;
    }
    ns->parent = parent;
    ns->table = symtable_init();
    if (!ns->table)
    {
        vec_destroy(ns->nodes);
        vec_destroy(ns->children);
        free(ns);
        return NULL;
    }
    ns->_global = false;
    return ns;
}

void namespace_destroy(namespace *ns)
{
    symtable_destroy(ns->table);
    vec_destroy(ns->nodes); // each node pointer is freed by the parser
    free(ns);
}

bool namespace_add_node(namespace *ns, node *node)
{
    return vec_push(ns->nodes, &node);
}

symtable_entry *namespace_query_symtable(namespace *ns, slice name)
{
    namespace *curr = ns;
    symtable_entry *res = NULL;
    while (curr != NULL)
    {
        if ((res = symtable_get(curr->table, name)) == NULL)
        {
            if (curr->parent && curr->cont != curr->parent->cont && !curr->parent->_global)
                break;
            curr = curr->parent;
        }
        else
            break;
    }
    return res;
}