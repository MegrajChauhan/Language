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

bool init_file_context(file_context *fc, char *fname)
{
    fc->children = NULL;
    fc->col = 0;
    fc->entry.fname = strdup(fname);
    if (!fc->entry.fname)
        return false;
    // it is the reader's job to verify the file and open it
    fc->line = 0;
    fc->offset = 0;
    fc->parent = NULL; // set manually
    return true;
}