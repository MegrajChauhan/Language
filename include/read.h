#ifndef _READ_
#define _READ_

#include "defs.h"
#include "file_context.h"
#include "uset.h"
#include "maps.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define rlog(msg) fprintf(stderr, "ERROR: %s.\n", msg)

// if this module encounters error, it prints just that and terminates(no special stuff)

bool create_file_context(file_context *fc); // needs the path to be present

bool is_file_valid(char *fname);

void pr_err(file_context *fc);

bool gen_file_context_stream(file_context *fc);

bool gen_file_context_tree(file_context *entry, uset *inc_list); // perform first pass

#endif