#ifndef _LEXER_
#define _LEXER_

#include <stdlib.h>
#include "defs.h"
#include "maps.h"
#include "error.h"

/**
 * The lexer is directly managed by the Parser.
 * We parse as well as lex at the same time.
 * This is more memory-efficient at the cost of performance.
 * This also provides us with a lot of benefits in the long term.
 */

typedef struct lexer lexer;
typedef struct token token;

struct lexer
{
    file_context *context; // the only file this lexer works on
    error *err;
    char *iter;
};

struct token
{
    uint64_t kind; // TokenType
    slice value;   // what is the value?
    size_t line;
    size_t col;
    size_t offset;
};

lexer *lexer_init(file_context *fc, error *e);

bool lexer_next_token(lexer *l, token *t);

bool lexer_peek_token(lexer *l, token *t);

void destroy_lexer(lexer *l);

bool lexer_request_character(lexer *l, token *t);

bool lexer_request_string(lexer *l, token *t);

/* UTILITIES */
char peek(lexer *l);

void consume(lexer *l);

bool group_number(lexer *l, token *t);

bool group_characters(lexer *l, token *t);

#endif