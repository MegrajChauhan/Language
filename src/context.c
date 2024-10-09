#include "context.h"

context *create_context(char *input_fname)
{
    context *c = (context *)malloc(sizeof(context));
    if (c == NULL)
        return NULL;
    c->errors = error_init();
    if (c->errors == NULL)
    {
        free(c);
        return NULL;
    }
    c->fcont = (file_context *)malloc(sizeof(file_context));
    if (c->fcont == NULL)
    {
        error_destroy(c->errors);
        free(c);
        return NULL;
    }
    c->_included_files = uset_init(20);
    if (!c->_included_files)
    {
        error_destroy(c->errors);
        free(c->fcont);
        free(c);
        return NULL;
    }
    if (!init_file_context(c->fcont, input_fname))
    {
        uset_destroy(c->_included_files);
        error_destroy(c->errors);
        free(c->fcont);
        free(c);
        return NULL;
    }
    return c;
}

void destroy_context(context *c)
{
    destroy_file_context(c->fcont);
    uset_destroy(c->_included_files);
    error_destroy(c->errors);
    free(c);
}
