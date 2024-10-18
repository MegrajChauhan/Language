#ifndef _AST_
#define _AST_

#include "nodes.h"
#include "error.h"
#include "defs.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct ast ast;
typedef struct ast_node ast_node;

enum
{
    SIMPLE,  // the ast node is a simple value
    COMPLEX, // the ast node is complex i.e array indexing or pointer dereferences etc
    AST,     // the node itself is another AST
};

struct ast_node
{
    ast_node *left;
    ast_node *right;
    uint8_t kind;
    union
    {
        node *n;
        vec *nodes;
        ast *child;
    };
};

struct ast
{
    ast_node nodes;
    uint64_t kind; // inherit the expression's kind
};

// size_t get_precedence(uint64_t kind);

// size_t

ast *ast_init();

bool expr_to_ast(ast *tree, expression *expr, error *e);

void ast_destroy(ast *tree);

bool ast_array_length_expr(ast *tree, expression *expr, error *e);

expression_nodes *ast_find_lowest_precedence(expression *expr);

expression_nodes *ast_find_binary_minus(expression *expr);

/*

(), [],

*/

#endif