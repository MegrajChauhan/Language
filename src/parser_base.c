#include "parser_base.h"

parser_state *produce_parser_state()
{
    parser_state *p = (parser_state *)malloc(sizeof(parser_state));
    if (!p)
    {
        report_internal_error("Failed to allocate memory for PARSER STATE.");
        return NULL;
    }
    nil(p->suggestion);
    nil(p->notes);
    nil(p->extra_msg);
    return p;
}

void destroy_parser_state(parser_state *st)
{
    check_source(st);
    if (st->notes)
        vec_destroy(st->notes);
    nil(st->notes);
    if (st->suggestion)
        free(st->suggestion);
    nil(st->suggestion);
    if (st->extra_msg)
        free(st->extra_msg);
    nil(st->extra_msg);
    free(st);
}

void parser_state_populate_suggesstion(parser_state *st, char *suggestion)
{
    check_source(st);
    check_source(suggestion);

    // try and allocate the memory in 'st->suggestion'
    // assuming st->suggestion is already freed
    // After copying in to the error stack, the field is NULL anyway

    register size_t len = strlen(suggestion);
    if (len == 0)
        return;

    st->suggestion = (char *)malloc(len + 1);

    if (!st->suggestion)
    {
        report_internal_error("Failed to allocate memory for SUGGESTION message.");
        crash(); // We crash because this is an error handling function which itself shouldn't fail
    }

    memcpy(st->suggestion, suggestion, len);
    st->suggestion[len] = 0; // done
}

void parser_state_populate_extra_msg(parser_state *st, char *extra_msg)
{
    // The logic as parser_state_populate_suggestion
    check_source(st);
    check_source(extra_msg);

    register size_t len = strlen(extra_msg);
    if (len == 0)
        return;

    st->extra_msg = (char *)malloc(len + 1);

    if (!st->extra_msg)
    {
        report_internal_error("Failed to allocate memory for EXTRA MESSAGE.");
        crash(); // We crash because this is an error handling function which itself shouldn't fail
    }

    memcpy(st->extra_msg, extra_msg, len);
    st->extra_msg[len] = 0; // done
}

void parser_state_add_note(parser_state *st, char *msg)
{
    check_source(st);
    check_source(msg);
    check_ptr(st, notes);

    log("ADDING A NEW NOTE\n");
    register size_t len = strlen(msg);
    if (len == 0)
        return;

    base_note note;
    note.note_msg = (char *)calloc(1, len + 1);

    if (!note.note_msg)
    {
        report_internal_error("Failed to allocate memory for a NOTE");
        crash();
    }
    memcpy(note.note_msg, msg, len);

    if (!vec_push(st->notes, &note))
    {
        report_internal_error("Failed to add a NOTE");
        crash();
    }

    // done
}

void parser_state_ready_notes(parser_state *st)
{
    check_source(st);

    if (st->notes)
        return;

    st->notes = vec_init(sizeof(base_note), ASSUMED_MAX_NOTES);

    if (!st->notes)
    {
        report_internal_error("Failed to initialize NOTES for parser.");
        crash();
    }
}