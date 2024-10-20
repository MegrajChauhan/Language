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
    tree->root->left = NULL;
    tree->root->right = NULL;
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
    bool must_eval = true;

    // We build the AST in a simple enough way.
    // Just start looking for the operators with the least precedence
    // start with a '-'
    // just get something and start from there
    tree->root = ast_get_root_node(expr);

    if (expr->nodes->count == 1)
    {
        // we are done with the expression
        expression_nodes *ret = (expression_nodes *)vec_at(expr->nodes, 0);
        if (ast_is_node_oper(ret) || !ast_is_node_simple(ret))
        {
            // this last node cannot be an operator
            ast_report_operator(expr, ret, e, cont);
            return false;
        }
        tree->root->left = NULL;
        tree->root->right = NULL;
        tree->root->kind = SIMPLE;
        tree->root->n = ret;
        return true;
    }

    if (!tree->root)
    {
        // this is invalid expression
        ast_report_operator(expr, (expression_nodes *)vec_at(expr->nodes, 0), e, cont);
        return false;
    }
    return ast_build_tree(tree->root, expr, e, cont);
}

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
    else
    {
        free(new_node);
        return NULL;
    }
    return new_node;
}

bool ast_build_tree(ast_node *root, expression *expr, error *e, file_context *fcont)
{
    // using the bloody root, build the bloody tree
    size_t ind = vec_index_of(expr->nodes, root->n);
    if (expr->nodes->count <= 1)
    {
        // we are done with the expression
        if (expr->nodes->count != 0)
        {
            expression_nodes *ret = (expression_nodes *)vec_at(expr->nodes, 0);
            if (ast_is_node_oper(ret) || !ast_is_node_simple(ret))
            {
                ast_report_operator(expr, ret, e, fcont);
                return false;
            }
            root->left = NULL;
            root->right = NULL;
            root->kind = SIMPLE;
            root->n = ret;
            return true;
        }
    }
    // Now the nodes may be just an array indexing
    vec prevec, subvec;
    vec_prevec(expr->nodes, &prevec, ind);
    vec_subvec(expr->nodes, &subvec, ind);
    expression left, right;
    left.nodes = &prevec;
    left.parent = expr->parent;
    left._type = expr->_type;
    right.nodes = &subvec;
    right.parent = expr->parent;
    right._type = expr->_type;
    root->left = ast_get_root_node(&left);
    if (!root->left && prevec.count > 1)
        return false;
    root->left = (ast_node *)malloc(sizeof(ast_node));
    if (!root->left)
    {
        internal_err();
        return false;
    }
    root->right = ast_get_root_node(&right);
    if (!root->right && subvec.count > 1)
    {
        free(root->left);
        return false;
    }
    root->right = (ast_node *)malloc(sizeof(ast_node));
    if (!root->right)
    {
        free(root->left);
        internal_err();
        return false;
    }
    return (ast_build_tree(root->left, &left, e, fcont) && ast_build_tree(root->right, &right, e, fcont));
}