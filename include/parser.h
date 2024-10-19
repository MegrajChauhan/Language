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

    // status flags
    bool _stat;
    bool error;
    bool f1, f2, f3; // any interpretation based on context
};

parser *parser_init(file_context *fc, error *e, namespace *ns);

void parser_destroy(parser *p);

bool parse(parser *p);

bool parse_add_expression(parser *p, expression *expr, uint64_t until);

bool parse_var_declr(parser *p, bool _const, token *old_tok);

bool parser_gen_type(parser *p, type *t, node *parent);

bool parser_check_token(parser *p, token *t, uint64_t exp);

#endif