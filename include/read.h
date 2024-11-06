#ifndef _READ_
#define _READ_

/**
 * Given a path, read the file and return a populated stream
 */

#include "report.h"
#include "stream.h"
#include "utils.h"
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

typedef struct file file;

// represents a read file
struct file
{
    stream *fdata;
};

file *file_read(char *path);

void file_close(file *f);

#endif