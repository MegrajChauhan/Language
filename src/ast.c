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
    case IDENTIFIER:
        return true;
    }
    return false;
}

static bool ast_is_node_oper(expression_nodes *n)
{
    uint64_t temp;
    return find_oper(&n->val, &temp);
}

ast *ast_init()
{
    ast *tree = (ast *)malloc(sizeof(ast));
    if (!tree)
        return NULL;
    return tree;
}

bool expr_to_ast(ast *tree, expression *expr, error *e, file_context *cont)
{
    // build the AST using associativity and precedence
    tree->kind = expr->_type;
    switch (expr->_type)
    {
    case ARR_LENGTH:
    case NORMAL_EXPR:
        if (!ast_array_length_expr(tree, expr, e, cont))
            return false;
        break;
    }
    return true;
}

void ast_destroy(ast *tree)
{
    ast_node *l = tree->root->left;
    ast_node *r = tree->root->right;
    if (l && r)
    {
        ast_node_destroy(l);
        ast_node_destroy(r);
    }
    free(tree->root);
    free(tree);
}

bool ast_array_length_expr(ast *tree, expression *expr, error *e, file_context *cont)
{
    // In this case, the expression cannot be extremely complicated
    // It should be simple enough
    // bool must_eval = true;
    return (tree->root = ast_build_tree(expr, e, cont)) != NULL;
}

bool ast_is_operator(expression_nodes *n)
{
    uint64_t temp = 0;
    if (!find_oper(&n->val, &temp))
        return false; // not an operator
    return true;
}

void ast_report(expression *expr, expression_nodes *err_node, error *e, file_context *cont)
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
    if (ind == 0 || ind >= (expr->nodes->count - 1))
        return false;
    expression_nodes *left = (expression_nodes *)vec_at(expr->nodes, ind - 1);
    expression_nodes *right = (expression_nodes *)vec_at(expr->nodes, ind + 1);
    if (!ast_is_operator(left) || !ast_is_operator(right))
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

ast_node *ast_get_root_node(expression *expr)
{
    ast_node *new_node = (ast_node *)malloc(sizeof(ast_node));
    if (!new_node)
    {
        internal_err();
        return NULL;
    }
    expression_nodes *root = NULL;
    new_node->kind = OPER;
    new_node->left = NULL;
    new_node->right = NULL;
    if ((root = ast_find_node(expr, OR)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, XOR)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, AND)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LSHIFT)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, RSHIFT)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, PLUS)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MINUS)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MUL)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, DIV)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MOD)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NOT)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, UNARY_MINUS)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, UNARY_PLUS)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, INC)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, DEC)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, OPEN_PAREN)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NUM_INT)) != NULL)
    {
        new_node->kind = SIMPLE;
        new_node->n = root;
    }
    else if ((root = ast_find_node(expr, NUM_FLOAT)) != NULL)
    {
        new_node->kind = SIMPLE;
        new_node->n = root;
    }
    else
    {
        free(new_node);
        return NULL;
    }
    return new_node;
}

ast_node *ast_build_tree(expression *expr, error *e, file_context *fcont)
{
    // using the bloody root, build the bloody tree
    ast_node *root = ast_get_root_node(expr);
    if (!root)
    {
        // error
        ast_report(expr, (expression_nodes *)vec_at(expr->nodes, 0), e, fcont);
        return NULL;
    }

    size_t ind = vec_index_of(expr->nodes, root->n);

    // Now the nodes may be just an array indexing
    vec prevec, subvec;
    expression left, right;

    vec_prevec(expr->nodes, &prevec, ind);
    vec_subvec(expr->nodes, &subvec, ind);

    left.nodes = &prevec;
    left.parent = expr->parent;
    left._type = expr->_type;

    right.nodes = &subvec;
    right.parent = expr->parent;
    right._type = expr->_type;

    if (subvec.count > 0)
    {
        root->right = ast_build_tree(&right, e, fcont);
        if (!root->right)
        {
            free(root);
            return NULL;
        }
        if (prevec.count > 0)
        {
            root->left = ast_build_tree(&left, e, fcont);
            if (!root->left)
            {
                ast_node_destroy(root->right);
                free(root);
                return NULL;
            }
        }
    }
    return root;
}