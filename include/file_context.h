#ifndef _FILE_CONTEXT_
#define _FILE_CONTEXT_

#include "read.h"
#include "utils.h"
#include "report.h"
#include "vec.h"
#include "slice.h"
#include "error.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct file_context file_context;

#include "comp_context.h"

/**
 * A file_context represents a single file.
 * Everything for a file belongs here including the symbol table, nodes list, parser, lexer etc
 */
struct file_context
{
    file *fstream;
    vec *children;
    file_context *parent;
    bool _stream_populated;
    slice *file_name;
    error *err;
    // parser *_p;
    // namespace *ns;
};

file_context *file_context_create(file_context *parent);

bool file_context_populate_stream(file_context *fcont, slice *file_path);

void file_context_destroy(void *fcont);

bool file_context_add_child(file_context *fcont);

#endif