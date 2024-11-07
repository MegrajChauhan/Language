#ifndef _COMP_CONTEXT_
#define _COMP_CONTEXT_

// represents a single compilation context
#include "compiler.h"
#include "utils.h"
#include "defs.h"
#include "report.h"
#include "vec.h"
#include "umap.h"
#include "uset.h"
#include "stack.h"
#include "stream.h"
#include "slice.h"
#include "queue.h"
#include "file_context.h"

typedef struct comp_context comp_context;

struct comp_context
{
    vec *_file_contexts;                // all file contexts
    umap *_fname_to_file_context;       // key = file path, value = index
    file_context *current_file_context; // the file being evaluated currently
    umap *_keywords;                    // map of all keywords
    umap *_operators;                   // map of all operators

    /**
     * While parsing, we parse one file at a time meaning even if we encounter an include statement, we won't go parse it first.
     * This allows for us to save some time in preparing the new context(we will create a new context but won't switch to it).
     * But this won't work while doing semantic analysis. Hence, we keep a stack of all file contexts for quick switching.
     */
    stack *_compilation_stack;
};

#endif