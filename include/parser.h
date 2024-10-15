#ifndef _PARSER_
#define _PARSER_

#include "defs.h"
#include "error.h"
#include "lexer.h"
#include "file_context.h"
#include "slice.h"
#include "vec.h"
#include "nodes.h"
#include "namespace.h"

typedef struct parser parser;

struct parser
{
    lexer *lex;
    error *err;
    namespace *ns; // the first one is unnamed which is the global namespace for one file
                   // one included file is a child namespace for the main file
};

parser *parser_init(file_context *fc, error *e, namespace *ns);

void parser_destroy(parser *p);

bool parse(parser *p);

bool parse_var_declr(parser *p, bool _const);

bool parser_gen_type(parser *p, type *t);

bool parser_check_token(parser *p, token *t, uint64_t exp);

#endif