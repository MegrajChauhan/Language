#ifndef _TYPES_
#define _TYPES_

#include "defs.h"
#include "nodes.h"
// #include "error.h"
// #include "namespace.h"

bool deduce_expression_type(type *type_of_var, expr_t *res);

bool is_type_array(type *type_to_check);

size_t get_dimension_of_array(type *array_type);

#endif