#ifndef _LANG_
#define _LANG_

#include "compiler.h"
#include "defs.h"
#include "utils.h"
#include "report.h"
#include "uset.h"
#include "umap.h"
#include "vec.h"
#include "slice.h"
#include "queue.h"
#include "stack.h"
#include "stream.h"
#include "enums.h"
#include "common_hdlr.h"
#include "lexer_hdlr.h"
#include "global_hdlr.h"
#include "error.h"
#include "keys.h"
#include "file_context.h"
#include "comp_context.h"
#include "global_context.h"
#include "read.h"
#include "lexer.h"

bool lang_main(int argc, char **argv);

void help();

void version();

#endif