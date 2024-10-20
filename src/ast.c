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

static bool ast_is_node_simple(expression_nodes *n)
{
    switch (n->type)
    {
    case NUM_INT:
    case NUM_FLOAT:
        return true;
    }
    return false;
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

bool expr_to_ast(ast *tree, expression *expr, error *e, file_context *cont)
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

bool ast_array_length_expr(ast *tree, expression *expr, error *e, file_context *cont)
{
    // In this case, the expression cannot be extremely complicated
    // It should be simple enough
    bool must_eval = true;

    // We build the AST in a simple enough way.
    // Just start looking for the operators with the least precedence
    // start with a '-'
    // just get something and start from there
}

// expression_nodes *ast_find_lowest_precedence(expression *expr)
// {
//     // go through the nodes in the expression and find the one
// }

bool ast_is_operator(expression_nodes *n)
{
    uint64_t temp = 0;
    if (!find_oper(&n->val, &temp))
        return false; // not an operator
    return true;
}

void ast_report_operator(expression *expr, expression_nodes *err_node, error *e, file_context *cont)
{
    error_inval_expr err;
    err.err_off_ed = err_node->offed;
    err.err_off_st = err_node->offst;
    err.expr = expr;
    error_add_complex(e, &err, cont, INVALID_EXPR);
}

bool ast_is_binary_oper(expression *expr, expression_nodes *n)
{
    size_t ind = vec_index_of(expr->nodes, n);
    if (ind == 0 || ind >= (expr->nodes->count))
        return false;
    expression_nodes *left = (expression_nodes *)vec_at(expr->nodes, ind - 1);
    expression_nodes *right = (expression_nodes *)vec_at(expr->nodes, ind + 1);
    if (ast_is_operator(left) || ast_is_operator(right))
        return false;
    return true;
}

expression_nodes *ast_find_node(expression *expr, uint64_t kind)
{
    for (size_t i = 0; i < expr->nodes->count; i++)
    {
        expression_nodes *n = (expression_nodes *)vec_at(expr->nodes, i);
        if (n->type == kind)
            return n;
    }
    return NULL; // not found
}

ast_node *ast_get_root_node(expression *expr, error *err, file_context *cont)
{
    ast_node *new_node = (ast_node *)malloc(sizeof(ast_node));
    if (!new_node)
    {
        internal_err();
        return NULL;
    }
    expression_nodes *root = NULL;
    if ((root = ast_find_node(expr, OR)) != NULL)
    {
    }
}

bool ast_form_node_binary(expression *expr, expression_nodes *oper, ast_node *new_node, error *err, file_context *cont)
{
    if (!ast_is_binary_oper(expr, oper))
        return false;
    size_t ind = vec_index_of(expr->nodes, oper);
    if (ind == 0 || ind >= (expr->nodes->count))
        return false;
    expression_nodes *left = (expression_nodes *)vec_at(expr->nodes, ind - 1);
    expression_nodes *right = (expression_nodes *)vec_at(expr->nodes, ind + 1);
    // since this is a binary operator, we will verify the left and right nodes
    // These nodes can be as complex as array indexing, pointer derefrencing and such
    // and so we will have to handle all
    // for now we don't have to worry about pointers
    bool left_simplicity = false, right_simplicity = false;
    left_simplicity = ast_is_node_simple(left);
    right_simplicity = ast_is_node_simple(right);
    if (left_simplicity)
    {
        
    }
    return true;
}