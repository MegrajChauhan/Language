#ifndef _AST_
#define _AST_

#include "nodes.h"
#include "error.h"
#include "defs.h"
#include "maps.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct ast ast;
typedef struct ast_node ast_node;

enum
{
    SIMPLE, // the ast node is a simple value
    OPER,
    ARRAY_INDEXING,
    AST,  // the node itself is another AST
    IDEN, // The node is a variable
    CHILD_EXPR,
};

struct ast_node
{
    ast_node *left;
    ast_node *right;
    uint8_t kind;
    expression_nodes *n;
    ast *child;
};

struct ast
{
    ast_node *root;
    uint64_t kind; // inherit the expression's kind
};

// size_t get_precedence(uint64_t kind);

// size_t

ast *ast_init();

bool expr_to_ast(ast *tree, expression *expr, error *e, file_context *cont);

void ast_destroy(ast *tree);

bool ast_array_length_expr(ast *tree, expression *expr, error *e, file_context *cont);

// expression_nodes *ast_find_lowest_precedence(expression *expr);
bool ast_is_operator(expression_nodes *n);

void ast_report(expression *expr, expression_nodes *err_node, error *e, file_context *cont);

bool ast_is_binary_oper(expression *expr, expression_nodes *n);

expression_nodes *ast_find_node(expression *expr, uint64_t kind);

expression_nodes *ast_find_node_ref(expression *expr, uint64_t kind, uint64_t ref);

expression_nodes *ast_next_node(expression *expr, expression_nodes *ref);

ast_node *ast_get_root_node(expression *expr);

ast_node *ast_build_tree(expression *parent, expression *expr, error *e, file_context *fcont);

bool ast_replace_paren(expression *parent, expression *expr, error *e, file_context *cont);

bool ast_handle_possible_identifiers(expression *expr, error *e, file_context *cont);

ast_node *ast_handle_different_nodes(ast_node *n, error *e, file_context *cont);

// bool ast_form_node_binary(expression *expr, expression_nodes *oper, ast_node *new_node, error *err, file_context *cont);
/*

(), [],

*/

#endif