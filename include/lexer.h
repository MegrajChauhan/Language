#ifndef _LEXER_
#define _LEXER_

#include "read.h"
#include "utils.h"
#include "report.h"
#include "enums.h"
#include "error.h"
#include "slice.h"
#include "global_hdlr.h"
#include "global_context.h"
#include "base.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct token token;
typedef struct lexer lexer;

// lexer state
typedef base_state lexer_state;

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

lexer *lexer_init(file *_f);

bool next_token(lexer *l, token *t);

void update_lexer(lexer *l);

token_t identify_number_type(lexer *l);

bool handle_decimal(lexer *l, token *t);

bool handle_binary(lexer *l, token *t);

bool handle_hex(lexer *l, token *t);

bool handle_oct(lexer *l, token *t);

bool get_number(lexer *l, token *t);

bool get_identifier(lexer *l, token *t);

void lexer_destroy(lexer *l);

void lexer_add_error(lexer *l, error_t type, slice *value, __error_hdlr hdlr);

#endif