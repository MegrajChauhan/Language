#ifndef _GLOBAL_CONTEXT_
#define _GLOBAL_CONTEXT_

// This includes all of the global variables usable for all things

#include "umap.h"
#include "utils.h"
#include "enums.h"
#include "slice.h"
#include "report.h"
#include <string.h>
#include <stdlib.h>

static umap *keys = NULL;

bool populate_keys();

void destroy_keys();

#endif