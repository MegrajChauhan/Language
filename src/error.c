#include "error.h"

error *error_init()
{
    error *e = (error *)malloc(sizeof(error));
    if (!e)
    {
        report_internal_error("Failed to allocate error handler.");
        return NULL;
    }
    e->errors = queue_create(free);
    if (!e->errors)
        return NULL;
    return e;
}

void error_add_entry(error *e, void *state, void *component, __error_hdlr hdlr)
{
    _error_entry *ent = (_error_entry *)malloc(sizeof(_error_entry));
    if (!ent)
    {
        report_internal_error("Error handler error.");
        crash(); // this is fatal
    }
    ent->component = component;
    ent->hdlr = hdlr;
    ent->state = state;
    if (!queue_enqueue(e->errors, ent))
    {
        report_internal_error("Error handler error.");
        crash(); // fatal
    }
}

void error_handle(error *e)
{
    set_printer_state(true);
    while (!queue_is_empty(e->errors))
    {
        _error_entry *ent = queue_dequeue(e->errors);
        e->_fatality = ent->hdlr(ent->state, ent->component);
        free(ent);
    }
    set_printer_state(false);
}

void error_destroy(error *e)
{
    check_ptr(e, errors);
    queue_destroy(e->errors);
    nil(e->errors);
    free(e);
}