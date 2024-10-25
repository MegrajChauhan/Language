#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "defs.h"
#include "vec.h"
#include "symtable.h"

#ifndef NONO
#define Assert(cond) ((cond) ? 0 : fprintf(stderr, "Assertion '%s' failed.\nFILE: %s\nLINE: %lu\n", #cond, __FILE__, __LINE__))
#else
#define Assert(cond)
#endif

typedef enum error_t error_t;

// Error kind(should give all the needed context)
enum error_t
{
    CANNOT_BUILD_TOKEN,
    DOUBLE_DOTS_FLOAT, // more than one '.' in a floating point number

    UNEXPECTED_TOKEN,
    UNEXPECTED_TOKEN_EXPR,
    UNEXPECTED_TOKEN_TYPE, // expected type
    UNEXPECTED_EOF,
    UNEXPECTED_EOL, // unexpected end of line(primarily for strings)

    INVALID_STRING,

    REDECLARATION,
    INVALID_TYPE_EXPR,
    INVALID_EXPR,

    UNARY_OPER_MULTIPLE_OPERAND,
    STRAY_CLOSING_PARENTHESIS,
    STARY_OPENING_PARENTHESIS,
    STRAY_OPEN_BIGBRAC,
    STRAY_CLOSE_BIGBRAC,
};

typedef struct error error;
typedef struct error_entry error_entry;
typedef struct error_unexp_tok error_unexp_tok;
typedef struct error_redeclr error_redeclr;
typedef struct error_inval_type_expr error_inval_type_expr;
typedef struct error_inval_expr error_inval_expr;

struct error_entry
{
    file_context *error_context; // the file where the error occured
    error_t kind;               // kind of error(for context)

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

struct error_redeclr
{
    node *redec;
    symtable_entry *original;
};

struct error_inval_type_expr
{
    type *_the_node_;
    type *st;
    node *n;
};

struct error_inval_expr
{
    expression *expr;
    size_t err_off_st, err_off_ed;
};

struct error
{
    vec *entries;
};

error *error_init();

void error_add(error *e, void *err, file_context *fcont, error_t kind, size_t els, size_t ele, size_t os, size_t oe, size_t cs, size_t ce);

void error_add_complex(error *e, void *err, file_context *cont, error_t kind);

void error_evaluate(error *e);

void error_destroy(error *e);

void multi_line_err(error_entry *e);

void err_line_print(error_entry *e);

void __cannot_built_token(error_entry *e);

void __double_dots_float(error_entry *e);

void __unexpected_token(error_entry *e, char *exp);

void __inval_string(error_entry *e);

size_t get_err_len(error_t kind);

void __invalid_type_expr(error_entry *e);

void __invalid_expr(error_entry *e);

void __redeclration(error_entry *e);

void __unary_oper_multiple_operand(error_entry *e);

void __stray_opers(error_entry *e);

#endif