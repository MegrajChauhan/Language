#ifndef _LEXER_
#define _LEXER_

#include "read.h"
#include "utils.h"
#include "report.h"
#include "enums.h"
#include "error.h"
#include "slice.h"
#include "file_context.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct token token;
typedef struct lexer lexer;

// lexer state
typedef struct lexer_state lexer_state;

struct token
{
    token_t kind;
    slice value;
    size_t line, col;
};

struct lexer
{
    file *_f;
    size_t line, col;
    char curr;
};

struct lexer_state
{
    error_t kind;
    size_t line, col;
    slice val;
};

lexer *lexer_init(file *_f);

bool next_token(lexer *l, token *t);

void update_lexer(lexer *l);

void lexer_destroy(lexer *l);

#endif