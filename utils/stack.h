#ifndef _STACK_
#define _STACK_

#include "utils.h"
#include "report.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct stack stack;

struct stack
{
    void *buffer;
    size_t elem_count;
    size_t elem_len;
    size_t sp;
};

stack *stack_create(size_t elem_len, size_t elem_count);

bool stack_push(stack *stk, void *data);

void *stack_pop(stack *stk);

void *stack_peek(stack *stk);

void stack_destroy(stack *stk)

#endif