#include "global_hdlr.h"

void set_current_file_conext(file_context *fcont)
{
    // fmt_log("RESETTING CURRENT FILE CONTEXT: [FILE CHANGE: %s'%s' to '%s'%s]\n", BOLDWHITE, curr->file_name->_str)
    curr = fcont;
}

void add_error(void *state, void *comp, __error_hdlr hdlr)
{
    check_source(state);
    check_source(comp);
    check_source(curr);
    error_add_entry(curr->err, state, comp, hdlr);
}