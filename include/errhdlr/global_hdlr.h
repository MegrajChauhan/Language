#ifndef _GLOBAL_HDLR_
#define _GLOBAL_HDLR_

#include "file_context.h"
#include "lexer_hdlr.h"

static file_context *curr = NULL;

void set_current_file_conext(file_context *fcont);

void add_error(void *state, void *comp, __error_hdlr hdlr);

#endif