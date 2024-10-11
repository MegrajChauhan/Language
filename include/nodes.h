#ifndef _NODES_
#define _NODES_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "slice.h"
#include "vec.h"
#include "defs.h"

enum
{
    VAR_DECLR,
    ARR_DECLR,
};

typedef struct node node;
typedef struct expression expression;

typedef struct node_var_declr node_var_declr;
typedef struct node_arr_declr node_arr_declr;

struct node
{
    uint64_t kind;
    void *_node;
    size_t l_st, l_ed;
    size_t c_st, c_ed;
    size_t o_st, o_ed;
};

struct expression
{
    vec *tokens;
    uint64_t _type; // could be any kind
                    // an array assignment needs to be evaluated differently
                    // than a regular expression
    bool must_eval; // the expression must be evaluated at compile-time?
};

struct node_var_declr
{
    slice name;
    expression expr;
    type *_t;
    // in array declaration, the value in-between '[]'
    // is also an expression
    // but that happens only in the case of an array
    // having an extra field for arrays that is useless for others is a bit
    // too much so we will have a different node for this
    // Though the given length should have been stored as type information
    // we won't complicate it there
};

struct node_arr_declr
{
    node_var_declr _info; // the same info
    expression _len_expr;
};

#endif