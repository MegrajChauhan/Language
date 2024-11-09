#include "global_hdlr.h"

void set_current_file_conext(file_context *fcont)
{
    curr = fcont;
}

void add_error(void *state, void *comp, __error_hdlr hdlr)
{
    check_source(state);
    check_source(comp);
    check_source(curr);
    error_add_entry(curr->err, state, comp, hdlr);
}