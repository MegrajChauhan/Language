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
#include "ast.h"
#include "global_hdlr.h"

// Use the current file context and parse
// Make a call to this function once the current token is part of an expression 
ast *parse_ast(vec *expr);

bool build_ast_from_root(ast *tree, vec *expr);

#endif