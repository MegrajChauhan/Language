#ifndef _ENUMS_
#define _ENUMS_

typedef enum token_t token_t;
typedef enum error_t error_t;

enum token_t
{
    TOK_EOF,
    TOK_ERROR,
    TOK_NUM_DECIMAL,
    TOK_NUM_FLOAT,
    TOK_NUM_BINARY,
    TOK_NUM_OCTAL,
    TOK_NUM_HEX,
    TOK_IDENTIFIER,

    TOK_FIRST_KEY, // Pseudo key
    TOK_LAST_KEY, // Pseudo key
};

enum error_t
{
    __INVALID_FLOATING_POINT_NUMBER,
    __INVALID_NUMBER_BASE_TYPE,
};

#define KEYWORD_COUNT 10

#endif