#ifndef _AST_
#define _AST_

#include "nodes.h"
#include "error.h"
#include "defs.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct ast ast;
typedef struct ast_node ast_node;

struct ast_node
{
    ast_node *left;
    ast_node *right;
    node *n;
};

struct ast
{
    ast_node nodes;
};

// size_t get_precedence(uint64_t kind);

// size_t 

ast *ast_init();

bool expr_to_ast(ast *tree, expression *expr, error *e);

void ast_destroy(ast *tree);

/*

(), [], 

*/

#endif