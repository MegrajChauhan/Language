#ifndef _FILE_CONTEXT_
#define _FILE_CONTEXT_

#include "read.h"
#include "utils.h"
#include "report.h"
#include "vec.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct file_context file_context;

/**
 * A file_context represents a single file.
 * Everything for a file belongs here including the symbol table, nodes list, parser, lexer etc
 */
struct file_context
{
    file *fstream;
    vec *children;
    file_context *parent;
};

file_context *file_context_create();

#endif