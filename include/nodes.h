#ifndef _NODES_
#define _NODES_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "slice.h"
#include "vec.h"

enum
{
    VAR_DECLR,
};

typedef struct node node;

typedef struct node_var_declr node_var_declr;

struct node
{
    uint64_t kind;
    void *_node;
    size_t l_st, l_ed;
    size_t c_st, c_ed;
    size_t o_st, o_ed;
};

struct node_var_declr
{
    slice *name;
};

#endif