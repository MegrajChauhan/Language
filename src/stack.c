#include "stack.h"

stack *stack_create(size_t elem_len, size_t elem_count)
{
    stack *stk = (stack *)malloc(sizeof(stack));
    if (!stk)
    {
        report_internal_error("Failed to allocate memory for stack.");
        return NULL;
    }

    stk->buffer = malloc(elem_len * elem_count);
    if (!stk->buffer)
    {
        free(stk);
        report_internal_error("Failed to allocate memory for stack buffer.");
        return NULL;
    }

    stk->elem_len = elem_len;
    stk->elem_count = elem_count;
    stk->sp = 0;

    return stk;
}

bool stack_push(stack *stk, void *data)
{
    check_ptr(stk, buffer);

    if (stk->sp >= stk->elem_count)
    {
        report_internal_error("Stack overflow.");
        return false;
    }

    memcpy((char *)stk->buffer + stk->sp * stk->elem_len, data, stk->elem_len);
    stk->sp++;
    return true;
}

void *stack_pop(stack *stk)
{
    check_ptr(stk, buffer);

    if (stk->sp == 0)
    {
        return NULL; // Stack underflow
    }

    stk->sp--;
    return (char *)stk->buffer + stk->sp * stk->elem_len;
}

void *stack_peek(stack *stk)
{
    check_ptr(stk, buffer);

    if (stk->sp == 0)
    {
        return NULL; // Stack is empty
    }

    return (char *)stk->buffer + (stk->sp - 1) * stk->elem_len;
}

void stack_destroy(stack *stk)
{
    check_ptr(stk);
    free(stk->buffer);
    free(stk);
}
