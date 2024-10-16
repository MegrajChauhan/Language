#ifndef _CONTEXT_
#define _CONTEXT_

#include "vec.h"
#include "uset.h"
#include "read.h"
#include "error.h"
#include "file_context.h"
#include "lexer.h"
#include "namespace.h"
#include "parser.h"

typedef struct context context;

struct context
{
    uset *_included_files; // all of the included files
    file_context *fcont;   // the entry context
    error *errors;
    namespace *ns;
};

context *create_context(char *input_fname);

bool context_make_first_pass(context *c);

bool context_parse_file_tree(context *c);

void destroy_context(context *c);

#endif