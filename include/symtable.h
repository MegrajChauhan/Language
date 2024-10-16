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
    union
    {
        struct
        {
            slice value;
        };

        struct
        {
            void *ptr;
        };
    };
    bool _const; // is the entry a constant
    uint64_t kind;
};

struct symtable
{
    umap *entries;
};

symtable *symtable_init();

void symtable_destroy(symtable *table);

#endif