#ifndef _BASE_
#define _BASE_

#include "slice.h"
#include "enums.h"
#include "vec.h"

typedef struct base_state base_state;
typedef struct base_note base_note;

struct base_state
{
    error_t kind;
    size_t line, col;
    slice val;
};

struct base_note
{
    vec *note_msg;  // extra note messages
    vec *green_msg; // try to correct the error for the user
};

#endif