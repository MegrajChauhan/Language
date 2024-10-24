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

static bool ast_is_node_unary_oper(expression_nodes *n)
{
    switch (n->type)
    {
    case UNARY_MINUS:
    case UNARY_PLUS:
    case INC:
    case DEC:
    case NOT:
    case LNOT:
        return true;
    }
    return false;
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
    if (!ast_replace_paren(expr, expr, e, cont))
        return false;
    return (tree->root = ast_build_tree(expr, expr, e, cont)) != NULL;
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
    else if ((root = ast_find_node(expr, SUB_EXPR)) != NULL)
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

ast_node *ast_build_tree(expression *parent, expression *expr, error *e, file_context *fcont)
{
    // using the bloody root, build the bloody tree
    ast_node *root = ast_get_root_node(expr);
    if (!root)
    {
        // error
        ast_report(parent, (expression_nodes *)vec_at(expr->nodes, 0), e, fcont);
        return NULL;
    }

    if (root->kind == SUB_EXPR)
    {
        expression_nodes *sub = root->n;
        free(root);
        root = ast_build_tree(sub->sub_expr, sub->sub_expr, e, fcont);
        if (!root)
            return false;
        return root; // done right here
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

    if (ast_is_node_unary_oper(root->n))
    {
        // this means that we cannot have operands on both side
        // we won't have ++a and only have a++
        // pretty much all unary operators only expect operands on the right side
        bool _register_err_ = false;
        expression *_fault_ = NULL;
        if (root->n->type == INC || root->n->type == DEC)
        {
            if (right.nodes->count > 0)
            {
                _register_err_ = true;
                _fault_ = &right;
            }
        }
        else if (root->left)
        {
            _register_err_ = true;
            _fault_ = &left;
        }
        if (_register_err_)
        {
            error_inval_expr err;
            expression_nodes *st = root->n;
            err.err_off_st = st->offst;
            err.err_off_ed = st->offed;
            err.expr = parent;
            error_add_complex(e, &err, fcont, UNARY_OPER_MULTIPLE_OPERAND);
            ast_node_destroy(root);
            return NULL;
        }
    }

    if (subvec.count > 0)
    {
        root->right = ast_build_tree(expr, &right, e, fcont);
        if (!root->right)
        {
            free(root);
            return NULL;
        }
    }
    if (prevec.count > 0)
    {
        root->left = ast_build_tree(expr, &left, e, fcont);
        if (!root->left)
        {
            if (subvec.count > 0)
                ast_node_destroy(root->right);
            free(root);
            return NULL;
        }
    }
    return root;
}

bool ast_replace_paren(expression *parent, expression *expr, error *e, file_context *cont)
{
    expression_nodes *paren;
    while ((paren = ast_find_node(expr, OPEN_PAREN)) != NULL)
    {
        size_t start = vec_index_of(parent->nodes, paren) + 1;
        size_t ind = start;
        bool close_found = false;
        while (!close_found)
        {
            if (ind >= parent->nodes->count)
            {
                // we reached the end with no closing parenthesis
                error_inval_expr err;
                err.err_off_st = paren->offst;
                err.err_off_ed = paren->offed;
                err.expr = parent;
                error_add_complex(e, &err, cont, STARY_OPENING_PARENTHESIS);
                return false;
            }
            expression_nodes *curr = (expression_nodes *)vec_at(parent->nodes, ind);
            switch (curr->type)
            {
            case OPEN_PAREN:
            {
                expression sub;
                vec_subvec(parent->nodes, sub.nodes, ind);
                if (!ast_replace_paren(parent, &sub, e, cont))
                    return false;
                // after that call the current pointer now points to the new sub expression
                ind += ((expression *)paren->sub_expr)->nodes->count + 1;
                break;
            }
            case CLOSE_PAREN:
            {
                size_t end = ind;
                expression_nodes sub;
                sub.sub_expr = vec_create_sub(parent->nodes, start, end - 1);
                if (!sub.sub_expr)
                    return false;
                sub.type = SUB_EXPR;
                sub.offst = paren->offst;
                sub.offed = curr->offed;
                vec_remove(parent->nodes, start - 1, end, &sub);
                close_found = true;
                break;
            }
            }
            ind++;
        }
    }
    if ((paren = ast_find_node(expr, CLOSE_PAREN)))
    {
        error_inval_expr err;
        err.err_off_st = paren->offst;
        err.err_off_ed = paren->offed;
        err.expr = parent;
        error_add_complex(e, &err, cont, STRAY_CLOSING_PARENTHESIS);
        return false;
    }

    return true;
}