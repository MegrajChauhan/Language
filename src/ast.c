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
    tree->kind = expr->_type;
    switch (expr->_type)
    {
    case ARR_LENGTH:
    }
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

bool ast_array_length_expr(ast *tree, expression *expr, error *e)
{
    // In this case, the expression cannot be extremely complicated
    // It should be simple enough
    bool must_eval = expr->must_eval;

    // We build the AST in a simple enough way.
    // Just start looking for the operators with the least precedence
}

// expression_nodes *ast_find_lowest_precedence(expression *expr)
// {
//     // go through the nodes in the expression and find the one
// }

expression_nodes *ast_find_binary_minus(expression *expr)
{
    // find the binary '-'
    for (size_t i = 0; i < expr->nodes->count; i++)
    {
        // since the associativity is from left to right 
        node *n = (node*)vec_at(expr->nodes, i);
        if (n->kind == MINUS)       
        {
            // confirm that this is a binary operator

            // if (i == 0)
            // {
            //     // unary '-'
            //     // this also works and so we need to add a 0 
            //     // the evaluator need to understand this itself
                
            // }
        }
    }
}