#ifndef _PARSER_
#define _PARSER_

#include "defs.h"
#include "error.h"
#include "lexer.h"
#include "file_context.h"
#include "slice.h"
#include "vec.h"
#include "nodes.h"

typedef struct parser parser;

struct parser
{
    lexer *lex;
    error *err;
    vec *nodes; // all of the nodes
};

#endif