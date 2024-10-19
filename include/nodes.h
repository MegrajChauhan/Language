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
};

enum
{
    ARR_LENGTH,
    ARR_ASSIGNMENT,
    NORMAL_EXPR,
    STRING_ASSIGN,
};

typedef struct type type;
typedef struct node node;
typedef struct expression_nodes expression_nodes;
typedef struct expression expression;

typedef struct node_var_declr node_var_declr;

struct node
{
    uint64_t kind;
    void *_node;
    size_t l_st, l_ed;
    size_t c_st, c_ed;
    size_t o_st, o_ed;
};

struct expression_nodes
{
    slice val;
    uint64_t type;
    size_t offst, offed;
};

struct expression
{
    vec *nodes;
    uint64_t _type; // could be any kind
                    // an array assignment needs to be evaluated differently
                    // than a regular expression
    bool must_eval; // the expression must be evaluated at compile-time?
    node *parent;
};

struct type
{
    uint64_t base;
    type *next;
    expression expr;
    size_t off, offe, col;
};

struct node_var_declr
{
    slice name;
    expression expr;
    type *_t;
    bool _const;
};

#endif