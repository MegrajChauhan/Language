#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "defs.h"
#include "vec.h"

#ifndef NONO
#define Assert(cond) ((cond)? 0: fprintf(stderr, "Assertion '%s' failed.\nFILE: %s\nLINE: %lu\n", #cond, __FILE__, __LINE__))
#else
#define Assert(cond)
#endif

// Error kind(should give all the needed context)
enum
{
    CANNOT_BUILD_TOKEN,
};

typedef struct error error;
typedef struct error_entry error_entry;

struct error_entry
{
    file_context *error_context; // the file where the error occured
    uint64_t kind;               // kind of error(for context)
    size_t err_line_st;
    size_t err_line_ed;
    size_t offset_st;
    size_t offset_ed;
    size_t col_st;
    size_t col_ed;
};

struct error
{
    vec *entries;
};

error *error_init();

bool error_add(error *e, file_context *fcont, uint64_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce);

// void error_evaluate(error* e);

void error_destroy(error *e);

#endif