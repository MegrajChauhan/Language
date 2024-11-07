#ifndef _LEXER_
#define _LEXER_

#include "read.h"
#include "utils.h"
#include "report.h"
#include "enums.h"
#include "error.h"
#include "slice.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct token token;
typedef struct lexer lexer;
typedef struct lexer_error lexer_error;

struct token
{
    token_t kind;
    slice *value;
    size_t line, col;
};

struct lexer
{
    file *_f;
    error *err;
};

struct lexer_error
{
    error_t kind;
};

#endif