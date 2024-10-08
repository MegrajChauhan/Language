#ifndef _DEFS_
#define _DEFS_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct file file;

struct file
{
    FILE *f;
    char *stream;
    size_t stream_len;
    file *parent;
};

#endif