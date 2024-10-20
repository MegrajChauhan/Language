#ifndef _DEFS_
#define _DEFS_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include "umap.h"

typedef struct file file;
typedef struct file_context file_context;

struct file
{
    FILE *f;
    char *stream;
    char *fname;
    size_t stream_len;
};

struct file_context
{
    file entry; // the file
    size_t line;
    size_t col;
    size_t offset;
    file_context *parent;
    file_context **children; // the child contexts
    size_t children_count;
};

// This contains just the definitions and no functions to manipulate them
// The using module does the manipulation

enum
{
    /* KEYS */
    // Types first
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    F32,
    F64,
    VOID,
    BOOL,
    TRUE,
    FALSE,

    // extra keywords
    INCLUDE,
    ASSEMBLY,

    // control-flow keywords
    IF,
    ELSE,

    // needed keywords(grammatical necessities)
    VAR,
    CONST,
    GLOBAL,
    FN,

    // Data structures
    ENUM,
    STRUCT,

    /* OPERATORS */

    /* single character operators */

    // grammer required operators
    COLON,
    SEMI_COLON,
    COMMA,

    // Assignment operators
    ASSIGN,

    // demand operators
    HASH_BLOCK,
    INC_DOLLAR,

    // absolutely necessary
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_CURLY,
    CLOSE_CURLY,
    OPEN_BIGBRAC,
    CLOSE_BIGBRAC,
    QUOTE,
    SINGLE_QUOTE, // we need to fit this somewhere
    ACCESS_DOT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    MOD,
    LNOT,
    OR,
    AND,
    NOT,
    XOR,
    GREATER_THAN,
    LESS_THAN,

    /* multi-character operators */

    // grammer required operators
    // Assignment operators
    PLUS_EQ,
    MINUS_EQ,
    MUL_EQ,
    DIV_EQ,
    MOD_EQ,
    OR_EQ,
    AND_EQ,
    NOT_EQ,
    XOR_EQ,
    LSHIFT_EQ,
    RSHIFT_EQ,

    // demand operators
    // absolutely necessary
    EQUALS,
    NOT_EQUALS,
    LOR,
    LAND,
    INC,
    DEC,
    LSHIFT,
    RSHIFT,
    GREATER_EQ,
    LESS_EQ,

    /* OTHER TOKEN TYPES */
    NUM_INT,
    NUM_FLOAT,
    STR,
    IDENTIFIER,

    REQUEST, // The parser makes a request for something to which this token is returned

    eof,

};

// Base types
enum
{
    SIGNED_BYTE,
    SIGNED_WORD,
    SIGNED_DWORD,
    SIGNED_QWORD,
    UNSIGNED_BYTE,
    UNSIGNED_WORD,
    UNSIGNED_DWORD,
    UNSIGNED_QWORD,
    FLOAT,
    DOUBLE,
    TCONST,
    ARRAY,
    TVOID, // this is for the functions only
    USER_DEF,
    CHAR,
    STRING,
};

#endif