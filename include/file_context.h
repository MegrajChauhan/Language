#ifndef _FILE_CONTEXT_
#define _FILE_CONTEXT_

#include "defs.h"

bool init_file_context(file_context *fc, char *fname);

void destroy_file_context(file_context *fc);

#endif