#ifndef _NAMESPACE_
#define _NAMESPACE_

// In this language a namespace is everything.
// An unnamed namespace is a scope
// Named scopes are special structures

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "defs.h"
#include "nodes.h"
#include "symtable.h"

typedef struct namespace namespace;

struct namespace
{
    vec *children;
    namespace *parent;
    // The namespace holds the nodes and the symbol table
    vec *nodes;
    symtable *table;
    slice name;   // name of the slice if it is named
    bool _global; // is the namespace global or not?
};

namespace *namespace_init(namespace *parent);

void namespace_destroy(namespace *ns);

#endif