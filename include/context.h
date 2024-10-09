#ifndef _CONTEXT_
#define _CONTEXT_

#include "vec.h"
#include "uset.h"
#include "read.h"
#include "error.h"
#include "file_context.h"

typedef struct context context;

struct context
{
    uset *_included_files; // all of the included files
    file_context *fcont;   // the entry context
    error *errors;
};

context *create_context(char *input_fname);

void destroy_context(context *c);

#endif