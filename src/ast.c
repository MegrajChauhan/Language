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
    return n->type == OPER;
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
    if (!ast_handle_possible_identifiers(expr, e, cont))
        return false;
    if (!ast_replace_paren(expr, expr, e, cont))
        return false;
    return (tree->root = ast_build_tree(expr, expr, e, cont)) != NULL;
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
    if (!ast_is_node_oper(left) || !ast_is_node_oper(right))
        return false;
    return true;
}

expression_nodes *ast_find_node_ref(expression *expr, uint64_t kind, astnode_t type, uint64_t ref)
{
    if (type == OPER || type == SYM || type == DATA)
    {
        for (size_t i = ref + 1; i < expr->nodes->count; i++)
        {
            expression_nodes *n = (expression_nodes *)vec_at(expr->nodes, i);
            if (n->tok_type == kind)
                return n;
        }
    }
    else
    {
        for (size_t i = ref + 1; i < expr->nodes->count; i++)
        {
            expression_nodes *n = (expression_nodes *)vec_at(expr->nodes, i);
            if (n->type == kind)
                return n;
        }
    }
    return NULL; // not found
}

expression_nodes *ast_find_node(expression *expr, uint64_t kind, astnode_t type)
{
    return ast_find_node_ref(expr, kind, type, (uint64_t)(-1));
}

expression_nodes *ast_next_node(expression *expr, expression_nodes *ref)
{
    size_t ind = vec_index_of(expr->nodes, ref);
    if ((ind + 1) >= expr->nodes->count)
        return NULL;
    return (expression_nodes *)vec_at(expr->nodes, ind + 1);
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
    new_node->left = NULL;
    new_node->right = NULL;
    if ((root = ast_find_node(expr, LOR, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LAND, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, OR, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, XOR, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, AND, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, EQUALS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NOT_EQUALS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LESS_THAN, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LESS_EQ, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, GREATER_THAN, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, GREATER_EQ, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LSHIFT, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, RSHIFT, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, PLUS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MINUS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MUL, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, DIV, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, MOD, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NOT, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, LNOT, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, UNARY_MINUS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, UNARY_PLUS, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, INC, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, DEC, OPER)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NUM_INT, DATA)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, NUM_FLOAT, DATA)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, VAR_NAME, DATA)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, ARRAY_INDEX, ARRAY_INDEX)) != NULL)
        new_node->n = root;
    else if ((root = ast_find_node(expr, SUB_EXPR, SUB_EXPR)) != NULL)
        new_node->n = root;
    else
    {
        free(new_node);
        return NULL;
    }
    return new_node;
}

ast_node *ast_handle_different_nodes(node *parent, ast_node *n, error *e, file_context *cont)
{
    ast_node *res = n;
    expression_nodes *sub = n->n;
    expression* tmp;
    switch (sub->type)
    {
    case SUB_EXPR:
        ast_node_destroy(n);
        tmp = &sub->_array_indexing_.index;
        res = ast_build_tree(tmp, tmp, e, cont);
        res->n = sub;
        break;
    case ARRAY_INDEX:
        tmp = &sub->_array_indexing_.index;
        ast *child_tree = ast_init();
        if (!child_tree)
        {
            internal_err();
            return NULL;
        }
        child_tree->kind = NORMAL_EXPR;
        child_tree->root = ast_build_tree(tmp, tmp, e, cont);
        if (!child_tree->root)
        {
            free(child_tree);
            return NULL;
        }
        res->child = child_tree;
        break;
    }
    return res;
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

    if (root->n->type == SUB_EXPR)
    {
        expression_nodes *sub = root->n;
        free(root);
        expression tmp = sub->_sub_expr_.sub_expr;
        root = ast_build_tree(&tmp, &tmp, e, fcont);
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
        root->right = ast_handle_different_nodes(parent->parent, root->right, e, fcont);
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
        root->left = ast_handle_different_nodes(parent->parent, root->left, e, fcont);
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
    while ((paren = ast_find_node(expr, OPEN_PAREN, OPER)) != NULL)
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
                // ind += ((expression *)paren->sub_expr)->nodes->count + 1;
                break;
            }
            case CLOSE_PAREN:
            {
                size_t end = ind;
                expression_nodes sub;
                sub._sub_expr_.sub_expr.nodes = vec_create_sub(parent->nodes, start, end - 1);
                if (!sub._sub_expr_.sub_expr.nodes)
                    return false;
                sub.type = SUB_EXPR;
                sub.offst = paren->offst;
                sub.offed = curr->offed;
                sub._sub_expr_.sub_expr.parent = parent->parent;
                sub._sub_expr_.sub_expr._type = NORMAL_EXPR;
                vec_remove(parent->nodes, start - 1, end, &sub);
                close_found = true;
                break;
            }
            }
            ind++;
        }
    }
    if ((paren = ast_find_node(expr, CLOSE_PAREN, OPER)))
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

bool ast_handle_possible_identifiers(expression *expr, error *e, file_context *cont)
{
    // we will specifically handle identifiers here.
    expression_nodes *id;
    id = ast_find_node(expr, IDENTIFIER, SYM);
    if (!id)
        return true;
    while (true)
    {
        if (!id)
            break;
        size_t ind = vec_index_of(expr->nodes, id);
        expression_nodes *nxt = ast_next_node(expr, id);
        if (!nxt)
            // this is it
            break;
        if (nxt->tok_type == ACCESS_DOT)
        {
            // This will be useful with structs
            // But we don't have that yet
        }
        else if (nxt->tok_type == OPEN_BIGBRAC)
        {
            // an array indexing
            size_t count = 1;
            expression_nodes *curr = nxt;
            while (count != 0)
            {
                curr = ast_next_node(expr, curr);
                if (!curr)
                    break;
                if (curr->tok_type == OPEN_BIGBRAC)
                    count++;
                else if (curr->tok_type == CLOSE_BIGBRAC)
                    count--;
            }
            if (count != 0)
            {
                error_inval_expr err;
                err.err_off_st = nxt->offst;
                err.err_off_ed = nxt->offed;
                err.expr = expr;
                error_add_complex(e, &err, cont, STRAY_OPEN_BIGBRAC);
                return false;
            }
            expression_nodes new;
            new.offed = curr->offed;
            new.offst = id->offst;
            new.type = ARRAY_INDEX;
            new._array_indexing_.array_name = id->_var_or_value_or_oper_.name_or_value_or_oper;
            new._array_indexing_.index.nodes = vec_create_sub(expr->nodes, ind + 1, vec_index_of(expr->nodes, curr) - 1);
            if (!new._array_indexing_.index.nodes)
            {
                internal_err();
                return false;
            }
            new._array_indexing_.index.parent = expr->parent;
            new._array_indexing_.index._type = NORMAL_EXPR;
            vec_remove(expr->nodes, ind, vec_index_of(expr->nodes, curr), &new);
        }
        id = ast_find_node_ref(expr, IDENTIFIER, SYM, ind);
    }
    if ((id = ast_find_node_ref(expr, CLOSE_BIGBRAC, OPER, vec_index_of(expr->nodes, id))) != NULL)
    {
        error_inval_expr err;
        err.err_off_st = id->offst;
        err.err_off_ed = id->offed;
        err.expr = expr;
        error_add_complex(e, &err, cont, STRAY_CLOSE_BIGBRAC);
        return false;
    }
    return true;
}