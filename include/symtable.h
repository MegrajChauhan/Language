#ifndef _SYMTABLE_
#define _SYMTABLE_

#include <stdlib.h>
#include "defs.h"
#include "umap.h"
#include "nodes.h"

typedef struct symtable_entry symtable_entry;
typedef struct symtable symtable;

enum
{
    _FUNCTION,
    _VARIABLE,
    _ARRAY,
    _ENUM,
    _STRUCT,
    _NAMESPACE,
};

struct symtable_entry
{
    type *t; // Just take the one from the node
    void *ptr;
    bool _const; // is the entry a constant
    uint64_t kind;
};

struct symtable
{
    umap *entries;
};

symtable *symtable_init();

bool symtable_find(symtable *table, slice name);

symtable_entry *symtable_get(symtable *table, slice name);

bool symtable_add(symtable *table, symtable_entry *entry, slice name);

void symtable_destroy(symtable *table);

#endif