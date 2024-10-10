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

    /* multi-character operators */

    // grammer required operators
    // Assignment operators
    EQUALS,
    // demand operators
    // absolutely necessary

    /* OTHER TOKEN TYPES */
    NUM_INT,
    NUM_FLOAT,
    IDENTIFIER,
    
    REQUEST, // The parser makes a request for something to which this token is returned
    
};

#endif