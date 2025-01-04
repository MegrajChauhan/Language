#ifndef _AST_
#define _AST_

#include "defs.h"
#include "lexer.h"

typedef struct ast ast;
typedef struct ast_node ast_node;

struct ast_node
{
    union
    {
        struct
        {
            ast_node *left;
            ast_node *right;
        } leaves;
        ast *child;
    } __;
    token node_token;
    bool _is_child;
};

struct ast
{
    ast_node *root;
};

void clean_ast_node(ast_node *n);

void clean_ast(ast *tree);

ast_node *produce_new_node();

#endif