#ifndef _PARSER_BASE_
#define _PARSER_BASE_

#include "enums.h"
#include "base.h"
#include "vec.h"
#include "lexer.h"
#include "error.h"
#include "parser_hdlr.h"
#include "report.h"

typedef struct parser_state parser_state;
typedef struct parser_base parser_base;

struct parser_state
{
    // We haven't really used the 'type' field.
    // But it can be used for debugging purposes later on
    base_state base;

    // In this case, 'line' in base_state will refer to the starting line
    size_t line_ed;
    char *suggestion; // any extra suggestion
    vec *notes;       // extra notes
};

#endif