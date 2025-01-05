#ifndef _ENUMS_
#define _ENUMS_

typedef enum token_t token_t;
typedef enum error_t error_t;
typedef enum compilerState_t compilerState_t;
typedef enum node_t node_t;

enum compilerState_t
{
    START_COMPILATION,
    PARSE,
    CHILDREN_AVAI,
    DONE_COMPILING,
    INVALID,
};

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

    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_MOD,
    TOK_INC,
    TOK_DEC,
    TOK_SQ_BRAC_OPEN,  // [
    TOK_SQ_BRAC_CLOSE, // ]
    TOK_PAREN_OPEN,    // (
    TOK_PAREN_CLOSE,   // )
    TOK_COND_AND,      // && for conditional statements
    TOK_COND_OR,       // || for conditional statements
    TOK_GREATER,       // > for conditional statements
    TOK_GREATER_OR_EQ, // >= for conditional statements
    TOK_SMALLER,       // < for conditional statements
    TOK_SMALLER_OR_EQ, // <= for conditional statements
    TOK_COND_NOT,      // Logical NOT !
    TOK_IS_EQ,         // ==
    TOK_NOT_EQ,        // !=
    TOK_XOR,           // Bitwise XOR ^
    TOK_NOT,           // Bitwise NOT ~
    TOK_AND,           // Bitwise OR |
    TOK_OR,            // Bitwise AND &
    TOK_LSHIFT,        // Bitwise Left Shift <<
    TOK_RSHIFT,        // Bitwise Right Shift >>
    TOK_TYPE_MATCHES,  // Check if the types match <>

    TOK_FIRST_KEY, // Pseudo key
    TOK_TRUE,      // just '1'
    TOK_FALSE,     // just '0'
    TOK_LAST_KEY,  // Pseudo key
};

enum error_t
{
    __INVALID_FLOATING_POINT_NUMBER,
    __INVALID_NUMBER_BASE_TYPE,
    __INVALID_TOKEN,
};

enum node_t
{
    _
};

#define KEYWORD_COUNT 26

#endif