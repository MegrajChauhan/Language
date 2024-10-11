#include "file_context.h"

bool init_file_context(file_context *fc, char *fname)
{
    fc->children = NULL;
    fc->col = 0;
    fc->entry.fname = strdup(fname);
    if (!fc->entry.fname)
        return false;
    // it is the reader's job to verify the file and open it
    fc->line = 1;
    fc->offset = 0;
    fc->parent = NULL; // set manually
    fc->children_count = 0;
    return true;
}

bool init_file_context_slice(file_context *fc, slice *s)
{
    fc->children = NULL;
    fc->col = 0;
    fc->entry.fname = strndup(s->_s, s->_e - s->_s + 1);
    if (!fc->entry.fname)
        return false;
    fc->line = 0;
    fc->offset = 0;
    fc->parent = NULL; // set manually
    fc->children_count = 0;
    return true;
}

void destroy_file_context(file_context *fc)
{
    if (!fc->children)
    {
        for (size_t i = 0; i < fc->children_count; i++)
            destroy_file_context(fc->children[i]);
        free(fc->children);
    }
    free(fc->entry.stream);
    free(fc->entry.fname);
    free(fc);
}

bool add_new_child_file_context(file_context *parent, file_context *child)
{
    parent->children = (file_context **)realloc(parent->children, sizeof(file_context *) * parent->children_count + 1);
    if (!parent->children)
        return false;
    parent->children[parent->children_count] = child;
    parent->children_count++;
    child->parent = parent;
    return true;
}

file_context *new_file_context(slice *s)
{
    file_context *fc = (file_context *)malloc(sizeof(file_context));
    if (!fc)
        return NULL;
    if (!init_file_context_slice(fc, s))
    {
        free(fc);
        return NULL;
    }
    return fc;
}