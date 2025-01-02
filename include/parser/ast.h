#ifndef _AST_
#define _AST_

#include "defs.h"
#include "lexer.h"

typedef struct ast ast;
typedef struct ast_node ast_node;

struct ast_node
{
    ast_node left;
    ast_node right;
    token node_token;
};

struct ast
{
    ast_node root;
};

#endif