#ifndef _TYPES_
#define _TYPES_

#include "defs.h"
#include "nodes.h"
#include "error.h"
// #include "namespace.h"

bool deduce_expression_type(type *type_of_var, expr_t *res);

bool is_type_array(type *type_to_check);

size_t get_dimension_of_array(type *array_type);

// bool is_type_primitive(type *type_to_check);

bool is_array_fit_for_string(type *array_type);

bool deduce_variable_type(expression *expr, type *to_build, error *err, file_context *cont);

#endif