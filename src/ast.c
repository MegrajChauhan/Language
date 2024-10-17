#include "ast.h"

static void ast_node_destroy(ast_node *n)
{
    ast_node *l = n->left;
    ast_node *r = n->right;
    if (l && r)
    {
        ast_node_destroy(l);
        ast_node_destroy(r);
    }
    free(n);
}

ast *ast_init()
{
    ast *tree = (ast *)malloc(sizeof(ast));
    if (!tree)
        return NULL;
    tree->nodes.left = NULL;
    tree->nodes.right = NULL;
    tree->nodes.n = NULL;
    return tree;
}

bool expr_to_ast(ast *tree, expression *expr, error *e)
{
    // build the AST using associativity and precedence
}

void ast_destroy(ast *tree)
{
    ast_node *l = tree->nodes.left;
    ast_node *r = tree->nodes.right;
    if (l && r)
    {
        ast_node_destroy(l);
        ast_node_destroy(r);
    }
    free(tree);
}
