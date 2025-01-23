#ifndef _PARSER_BASE_
#define _PARSER_BASE_

#include "enums.h"
#include "base.h"
#include "vec.h"
#include "lexer.h"
#include "error.h"
#include "parser_hdlr.h"
#include "global_hdlr.h"
#include "report.h"

typedef struct parser_state parser_state;

struct parser_state
{
    // We haven't really used the 'type' field.
    // But it can be used for debugging purposes later on
    base_state base;

    // In this case, 'line' in base_state will refer to the starting line
    size_t line_ed;
    char *suggestion; // any extra suggestion
    char *extra_msg;  // extra error message
    vec *notes;       // extra notes

    __error_hdlr hdlr; // the chosen handler
};

/**
 * Error handling for complex components will be complex as well.
 * We have to generate error differently based on the context.
 */

parser_state *produce_parser_state();

void destroy_parser_state(parser_state *st);

// Exits on failure
void parser_state_populate_suggesstion(parser_state *st, char *suggestion);

// Exits on failure
void parser_state_populate_extra_msg(parser_state *st, char *extra_msg);

// char *parser_produce_suggestion_msg(char *fmt, slice *val);

// part of error handling. CAN CRASH
void parser_state_add_note(parser_state *st, char *msg);

// part of error handling. CAN CRASH
void parser_state_ready_notes(parser_state *st);

// part of error handling. CAN CRASH
void parser_state_clean_notes(parser_state *st);

#endif