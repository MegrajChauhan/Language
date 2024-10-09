#ifndef _DEFS_
#define _DEFS_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct file file;
typedef struct file_context file_context;

struct file
{
    FILE *f;
    char *stream;
    char* fname;
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
};

// This contains just the definitions and no functions to manipulate them
// The using module does the manipulation

#endif