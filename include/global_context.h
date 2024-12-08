#ifndef _GLOBAL_CONTEXT_
#define _GLOBAL_CONTEXT_

// This includes all of the global variables usable for all things

#include "umap.h"
#include "utils.h"
#include "enums.h"
#include "slice.h"
#include "queue.h"
#include "report.h"
#include <string.h>
#include <stdlib.h>

static umap *keys = NULL;
static compilerState_t cstate = INVALID;
static compilerState_t assumed_next_state = INVALID;

void set_compiler_state(compilerState_t state);

void set_assumed_state(compilerState_t state);

void set_assumed_state_as_compiler_state();

compilerState_t get_compiler_state();

bool populate_keys();

token_t is_a_key(slice *val);

void destroy_keys();

#endif