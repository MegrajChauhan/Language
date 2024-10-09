#include "file_context.h"

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