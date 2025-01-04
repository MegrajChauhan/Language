#include "ast.h"

void clean_ast_node(ast_node *n)
{
    if (n->_is_child)
        clean_ast(n->__.child);
    else
    {
        if (n->__.leaves.left)
            clean_ast_node(n->__.leaves.left);
        if (n->__.leaves.right)
            clean_ast_node(n->__.leaves.right);
    }
    free(n);
}

void clean_ast(ast *tree)
{
    check_source(tree);
    if (tree->root)
        clean_ast_node(tree->root);
    free(tree);
}

ast_node *produce_new_node()
{
    ast_node *n = (ast_node *)malloc(sizeof(ast_node));
    if (!n)
    {
        report_internal_error("Failed to allocate memory for a new ast node.");
        return NULL;
    }
    n->_is_child = false; // set by the caller
    n->__.leaves.left = n->__.leaves.right = NULL;
    return n;
}