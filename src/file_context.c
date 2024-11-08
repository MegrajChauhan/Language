#include "file_context.h"

file_context *file_context_create(file_context *parent)
{
    file_context *fcont = (file_context *)malloc(sizeof(file_context));
    if (!fcont)
    {
        report_internal_error("Failed to create new file context.");
        return NULL;
    }
    fcont->children = vec_init(sizeof(file_context *), ASSUME_CHILDREN_FILES_INCLUDED);
    if (!fcont->children)
    {
        report_internal_error("Failed to create new file context.");
        free(fcont);
        return NULL;
    }
    fcont->err = error_init();
    if (!fcont->err)
    {
        report_internal_error("Failed to create new file context.");
        vec_destroy(fcont->children);
        free(fcont);
        return NULL;
    }
    fcont->fstream = NULL; // not yet
    fcont->_stream_populated = false;

    fcont->parent = parent;
    return fcont;
}

bool file_context_populate_stream(file_context *fcont, slice *file_path)
{
    check_source(fcont);
    if (fcont->_stream_populated)
        return true;
    char *path = slice_to_str(file_path);
    if (!path)
        return false;
    fmt_log("POPULATING FILE STREAM: Path '%s'", path);
    fcont->fstream = file_read(path);
    if (!fcont->fstream)
    {
        fmt_log("POPULATING FILE STREAM(%sFailed%s): Path '%s'", BOLDRED, RESET, path);
        report_internal_error("Failed to populate file stream.");
        free(path);
        return false;
    }
    fmt_log("POPULATING FILE STREAM(%sSuccess%s): Path '%s'", BOLDGREEN, RESET, path);
    free(path);
    fcont->_stream_populated = true;
    fcont->file_name = file_path;
    return true;
}

void file_context_destroy(void *fcont)
{
    check_source(fcont);
    if (((file_context *)fcont)->_stream_populated)
        file_close(((file_context *)fcont)->fstream);
    if (((file_context *)fcont)->children)
    {
        vec_clean(((file_context *)fcont)->children, file_context_destroy);
        vec_destroy(((file_context *)fcont)->children);
    }
    if (((file_context *)fcont)->err)
        error_destroy(((file_context *)fcont)->err);
    free(fcont);
}

bool file_context_add_child(file_context *fcont)
{
    check_source(fcont);
    file_context *new_cont = file_context_create(fcont);
    if (!new_cont)
        return false;
    if (!vec_push(fcont->children, new_cont))
    {
        file_context_destroy(new_cont);
        return false;
    }
    return true;
}

void set_current_file_conext(file_context *fcont)
{
    current_active = fcont;
}