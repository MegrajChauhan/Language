#ifndef _PARSER_EXPR_
#define _PARSER_EXPR_

/**
 * Parse expressions here
 * The expressions that will be parsed is not defined yet.
 * Some of the expressions that have been defined are:
 * 1) Math expressions
 * 2) Conditional expressions
 * 3) Boolean expressions  
 */

#include "parser_base.h"
#include "report.h"
#include "lexer.h"
#include "error.h"
#include "vec.h"

typedef struct ast ast;
typedef struct ast_node ast_node;

struct ast_node
{
    token tok;
};

#endif