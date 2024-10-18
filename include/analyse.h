#ifndef _ANALYSE_
#define _ANALYSE_

// Analyse the nodes to generate the symbol table
// This is where we confirm everything
#include "nodes.h"
#include "defs.h"
#include "namespace.h"
#include "error.h"
#include <stdlib.h>

bool analyse(namespace *ns, error *e);

bool variable_declaration(namespace *ns, node *n, error *e);

bool analyse_type(namespace *ns,node *n, type *t, error *e);

#endif