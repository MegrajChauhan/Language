#ifndef _FILE_CONTEXT_
#define _FILE_CONTEXT_

#include "defs.h"

bool init_file_context(file_context *fc, char *fname);

bool init_file_context_slice(file_context *fc, slice *s);

void destroy_file_context(file_context *fc);

bool add_new_child_file_context(file_context *parent, file_context *child);

file_context *new_file_context(slice *s);

#endif