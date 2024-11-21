#ifndef _COMMON_HDLR_
#define _COMMON_HDLR_

#include "slice.h"
#include "utils.h"
#include "report.h"
#include <stdio.h>

#define define_errhdlr(name) void name(void *state, void *comp)

void print_filename_and_details(slice *filename, size_t col, size_t line, char *msg);

#endif