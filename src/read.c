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
    if (len < 3)
        return false;
    if (!(*(fname + (len - 1)) == 'l' && *(fname + (len - 2)) == '.'))
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
    {
        internal_err();
        return false;
    }
    fread(fc->entry.stream, 1, flen, fc->entry.f); // should not fail
    fc->entry.stream_len = flen;
    fc->entry.stream[flen - 1] = 0; // terminate
    return true;
}

static bool get_include_path(char **iter, size_t *l, slice *path, file_context *fc)
{
    char *i = *iter;
    while (IsSpace(*i) && *i != 0)
    {
        if (*i == '\n')
            (*l)++;
        i++;
    }
    if (*i != '(')
    {
        fprintf(stderr, "ERROR: Expected '(' after 'assembly' or 'include' but got '%c' instead. In file '%s', line %lu.\n", *i, fc->entry.fname, *l);
        return false;
    }
    // we may hit anything- a character, number or '../' of paths
    // This is where the slice starts and we read until we hit ')'
    /// NOTE: There cannot be spaces in between '()' as the path itself could have an space
    i++;
    path->_s = i;
    while (*i != ')' && *i != 0)
    {
        if (*i == '\n')
        {
            fprintf(stderr, "ERROR: Expected a path after 'assembly' or 'include' but got new-line instead. In file '%s', line %lu.\n", fc->entry.fname, *l);
            return false;
        }
        i++;
    }
    // no new-lines either otherwise they will be part of the path as well
    if (*i != ')')
    {
        fprintf(stderr, "ERROR: Expected ')' after path but got '%c' instead. In file '%s', line %lu.\n", *i, fc->entry.fname, *l);
        return false;
    }
    path->_e = i - 1;
    i++; // beyond the parenthesis
    *iter = i;
    return true;
}

bool gen_file_context_tree(file_context *entry, uset *inc_list)
{
    // first set-up entry
    slice p;
    p._s = entry->entry.fname;
    p._e = entry->entry.fname + (strlen(entry->entry.fname) - 1);
    get_filename_from_path(&p);
    if (uset_contains_slice(inc_list, &p))
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
    {
        internal_err();
        return false;
    }
    char *iter = entry->entry.stream;
    size_t lstart = 1;
    while (*iter != 0)
    {
        if (*iter == '\n')
        {
            iter++;
            lstart++;
        }
        else if (*iter == '/' && *(iter + 1) == '/')
            while (*iter != '\n' && *iter != '\0')
                iter++;
        else if (*iter == '@')
        {
            size_t temp = lstart;
            iter++;
            while (*iter != '@' && *iter != 0)
            {
                if (*iter == '\n')
                    lstart++;
                iter++;
            }
            if (*iter != '@')
            {
                // where is the '@'
                fprintf(stderr, "ERROR: Multi-line comment was not terminated. Stray '@' in file '%s', line %lu.\n", entry->entry.fname, temp);
                return false;
            }
            iter++;
        }
        else if (*iter == '#')
        {
            size_t temp = lstart;
            iter++;
            while (*iter != '#' && *iter != 0)
            {
                if (*iter == '\n')
                    lstart++;
                iter++;
            }
            if (*iter != '#')
            {
                // where is the '#'?
                fprintf(stderr, "ERROR: Block was never ended. Stray '#' in file '%s', line %lu.\n", entry->entry.fname, temp);
                return false;
            }
            iter++;
        }
        else if (*iter == '$')
        {
            // this is where we deal with the include
            size_t temp = lstart;
            iter++;
            while (IsSpace(*iter) && *iter != 0)
            {
                if (*iter == '\n')
                    lstart++;
                iter++;
            }
            if (iter == 0 || !IsAlpha(*iter))
            {
                fprintf(stderr, "ERROR: Expected 'include', 'assembly' or '..(in future)..' after '$' in file '%s', line %lu.\n", entry->entry.fname, temp);
                return false;
            }
            slice s;
            s._s = iter;
            while (IsAlpha(*iter) && *iter != 0)
            {
                iter++;
            }
            s._e = iter;
            slice path;
            if (strncmp(s._s, "assembly", s._e - s._s) == 0)
            {
                // an assembly(ignore right now but we should remember this somehow)
                /// TODO: Finish me
                if (!get_include_path(&iter, &lstart, &path, entry))
                    return false;
                // else
                // remember it somewhere
            }
            else if (strncmp(s._s, "include", s._e - s._s) == 0)
            {
                if (!get_include_path(&iter, &lstart, &path, entry))
                    return false;
                // if (init_file_context_slice())
                file_context *new;
                if ((new = new_file_context(&path)) == NULL)
                {
                    internal_err();
                    return false;
                }
                if (!create_file_context(new) || !gen_file_context_tree(new, inc_list))
                {
                    fprintf(stderr, "Included from '%s' at line %lu\n", entry->entry.fname, temp);
                    return false;
                }
                if (!add_new_child_file_context(entry, new))
                {
                    internal_err();
                    return false;
                }
            }
            else
            {
                fprintf(stderr, "ERROR: Expected 'assembly' or 'include' after '$'. In file '%s' line %lu\n", entry->entry.fname, lstart);
                return false;
            }
        }
        else
        {
            iter++;
        }
    }
    return true;
}