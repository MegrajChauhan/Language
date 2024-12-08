#ifndef _ERROR_
#define _ERROR_

#include "queue.h"
#include "report.h"
#include "utils.h"
#include "common_hdlr.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct _error_entry _error_entry;
typedef struct error error;
typedef bool (*__error_hdlr)(void *state, void *comp);

struct _error_entry
{
    void *state;
    void *component;
    __error_hdlr hdlr;
};

struct error
{
    queue *errors;
    bool _fatality;
};

error *error_init();

void error_add_entry(error *e,void *state, void *component, __error_hdlr hdlr);

void error_handle(error *e);

void error_destroy(error *e);

#endif