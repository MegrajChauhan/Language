#ifndef _EXPR_
#define _EXPR_

#include "nodes.h"
#include "defs.h"
#include "slice.h"
#include "lexer.h"
#include "namespace.h"
#include "ast.h"
#include "type.h"
#include <stdlib.h>

typedef struct expr_result expr_result;
typedef struct expr_state expr_state;

struct expr_result
{
    type *t;
    ast_node *val;
};

struct expr_state
{
    namespace *ns;
    error *e;
    ast *tree;
    node *parent;
    symtable_entry *last_var_entry; // for type checks
};

expr_result *evaluate_expression(ast *tree, namespace *ns, error *e, node *parent);

expr_result *simple_expression_evaluation(expr_state *state);

bool evaluate_node(ast_node *n, expr_state *state);

bool last_var_matches_this_type(expr_state *state, symtable_entry *ent);

void replace_var_value(symtable_entry *ent, ast_node *n);

#endif