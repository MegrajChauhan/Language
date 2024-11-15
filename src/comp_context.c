#include "comp_context.h"

comp_context *create_compilation_context()
{
    cont = (comp_context *)calloc(1, sizeof(comp_context));
    if (!cont)
    {
        report_internal_error("Failed to create compilation context.");
        return NULL;
    }
    cont->_all_used_files = uset_create(ASSUME_FILES_INCLUDED, slice_hash, slice_cmp, slice_destroy);
    if (!cont->_all_used_files)
        goto _err;
    return cont; // temp
_err:
    destroy_compilation_context();
    report_internal_error("Failed to create compilation context.");
    return NULL;
}

void destroy_compilation_context()
{
    check_source(cont);
    if (cont->root)
        file_context_destroy(cont->root);
    if (cont->_all_used_files)
        uset_destroy(cont->_all_used_files);
    free(cont);
    cont = NULL;
}

bool file_already_exists(slice *file_path)
{
    check_source(cont);
    return uset_contains(cont->_all_used_files, file_path);
}

bool add_file_context(slice *file_path, file_context *parent)
{
    check_source(cont);
    check_source(parent);
    if (file_already_exists(file_path))
        return true;
    if (!file_context_add_child(parent))
        return false;
    if (!uset_insert(cont->_all_used_files, file_path))
        return false;
}

bool add_base_file_context(file_context *base)
{
    check_source(cont);
    check_source(base);
    cont->root = base;
    if (!uset_insert(cont->_all_used_files, base->file_name))
        return false;
    return true;
}

bool compile()
{
    return true;
}