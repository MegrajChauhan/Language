#include "read.h"

// An undesirable return from any of the following indicates a fatal error and
// we terminate immediately

bool create_file_context(file_context *fc)
{
    // start with no children of course
    if (!is_file_valid(fc->entry.fname))
    {
        fprintf(stderr, "ERROR: The file '%s' is not a valid, supported file.\n", fc->entry.fname);
        return false;
    }
    fc->entry.f = fopen(fc->entry.fname, "r");
    if (!fc->entry.f)
    {
        pr_err(fc);
        return false;
    }
    if (gen_file_context_stream(fc) == false)
    {
        fclose(fc->entry.f);
        return false;
    }
    fclose(fc->entry.f);
    return true;
}

bool is_file_valid(char *fname)
{
    size_t len = strlen(fname);
    if (len < 2)
        return false;
    if (!(*(fname + len) == 'l' && (*fname + (len - 1)) == '.'))
        return false;
    return true;
}

void pr_err(file_context *fc)
{
    switch (errno)
    {
    case EISDIR:
        fprintf(stderr, "ERROR: The file '%s' is a directory.\n", fc->entry.fname);
        break;
    default:
        // most likely failed to open.
        fprintf(stderr, "ERROR: Failed to open file '%s'.\n", fc->entry.fname);
    }
}

bool gen_file_context_stream(file_context *fc)
{
    size_t flen = 0;
    fseek(fc->entry.f, SEEK_SET, SEEK_END);
    flen = ftell(fc->entry.f);
    rewind(fc->entry.f);
    fc->entry.stream = (char *)malloc(flen + 1);
    if (fc->entry.stream == NULL)
        return false;
    fread(fc->entry.stream, 1, flen, fc->entry.f); // should not fail
    fc->entry.stream_len = flen;
    fc->entry.stream[flen - 1] = 0; // terminate
    return true;
}

bool gen_file_context_tree(file_context *entry, uset *inc_list)
{
    // first set-up entry
    if (uset_contains(inc_list, entry->entry.fname))
        return true;
    // make the first pass and generate the tree
    if (!create_file_context(entry))
    {
        // failed to create the context
        // in this case, termination with error message is a must
        // but every such failure is an automatic "internal error"
        // we print it and exit
        return false; // immediate after-termination
    }
    // add to the uset
    if (!uset_insert(inc_list, entry->entry.fname))
        return false;
    register char *iter = entry->entry.stream;
    register size_t cstart = 0, lstart = 0, ostart = 0;
    while (*iter != 0)   
    {
              
    }
    return true;
}