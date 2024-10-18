#ifndef _EXPR_
#define _EXPR_

#include "nodes.h"
#include "defs.h"
#include "slice.h"
#include "lexer.h"
#include "namespace.h"
#include "ast.h"
#include <stdlib.h>

typedef struct expr_evaluator expr_evaluator;
typedef struct expr_result expr_result;

struct expr_result
{
    type *t;
    void *val;
};

struct expr_evaluator
{
    ast *tree;       // the tree to evaluate
    error *e;        // error handling
    expr_result res; // the result

    namespace *parent;
    // config
    node *res_type; // variable node
};

#endif