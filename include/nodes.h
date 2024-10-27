#ifndef _NODES_
#define _NODES_

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "slice.h"
#include "vec.h"
#include "defs.h"

typedef enum exprnode_t exprnode_t;
typedef enum node_t node_t;
typedef enum expr_t expr_t;

enum node_t
{
    VAR_DECLR,
};

enum expr_t
{
    ARR_LENGTH,
    NORMAL_EXPR,
    ARR_ASSIGNMENT,
    STR_ARR_ASSIGNMENT,
    MULTI_DIMENSION_ARR_ASSIGNMENT,
    STR_MULTI_DIMENSION_ARR_ASSIGNMENT, // special case(strings are acceptable as arrays)
    STRING_ASSIGN,
};

enum exprnode_t
{
    SUB_EXPR,
    SYM,
    DATA,
    ARRAY_INDEX,
    OPER,
};

typedef struct type type;
typedef struct node node;
typedef struct expression_nodes expression_nodes;
typedef struct expression expression;

typedef struct node_var_declr node_var_declr;

struct node
{
    node_t kind;
    void *_node;
    size_t l_st, l_ed;
    size_t c_st, c_ed;
    size_t o_st, o_ed;
};

struct expression
{
    vec *nodes;
    expr_t _type;   // could be any kind
                    // an array assignment needs to be evaluated differently
                    // than a regular expression
    bool must_eval; // the expression must be evaluated at compile-time?
    node *parent;
};

struct expression_nodes
{
    exprnode_t type;
    uint64_t tok_type;
    size_t offst, offed;
    union
    {
        struct
        {
            slice name_or_value_or_oper;
        } _var_or_value_or_oper_;

        struct
        {
            slice array_name;
            expression index;
        } _array_indexing_;

        struct
        {
            expression sub_expr;
        } _sub_expr_;
    };
};

struct type
{
    uint64_t base;
    type *next;
    expression expr;
    size_t off, offe, col;
    size_t _length_;
};

struct node_var_declr
{
    slice name;
    expression expr;
    type *_t;
    bool _const;
};

#endif