#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "defs.h"
#include "vec.h"

#ifndef NONO
#define Assert(cond) ((cond) ? 0 : fprintf(stderr, "Assertion '%s' failed.\nFILE: %s\nLINE: %lu\n", #cond, __FILE__, __LINE__))
#else
#define Assert(cond)
#endif

// Error kind(should give all the needed context)
enum
{
    CANNOT_BUILD_TOKEN,
    DOUBLE_DOTS_FLOAT, // more than one '.' in a floating point number

    UNEXPECTED_TOKEN,
    UNEXPECTED_TOKEN_TYPE, // expected type
    UNEXPECTED_EOF,
};

typedef struct error error;
typedef struct error_entry error_entry;
typedef struct error_unexp_tok error_unexp_tok;

struct error_entry
{
    file_context *error_context; // the file where the error occured
    uint64_t kind;               // kind of error(for context)

    // in case of lexers, both endings and startings would be the same
    // but this extra info will be useful with parsers and semantic analysis
    size_t err_line_st;
    size_t err_line_ed;
    size_t offset_st;
    size_t offset_ed;
    size_t col_st;
    size_t col_ed;

    void *err;
};

struct error_unexp_tok
{
    slice got;
    uint64_t exp; // we need to get a "TYPE to STR" thing now
};

struct error
{
    vec *entries;
};

error *error_init();

void error_add(error *e, void *err, file_context *fcont, uint64_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce);

void error_evaluate(error *e);

void error_destroy(error *e);

void __cannot_built_token(error_entry *e);

void __double_dots_float(error_entry *e);

void __unexpected_token(error_entry *e, char *exp);

#endif