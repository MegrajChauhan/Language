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

void handle_invalid_state()
{
    // The compiler will crash based on the situation here
    // If not initialized, exit with proper message
    // else, print the messages and continue

    // It is not possible that the compiler is unintialized
    // So, we can just go to errors
    file_context *current_context = get_current_file_context();
    error_handle(current_context->err);

    if (current_context->err->_fatality)
        crash();
    set_assumed_state_as_compiler_state(); // try to restore the compiler
}

bool compile()
{
    check_ptr(cont, root);
    set_current_file_conext(cont->root);

    log("Starting Compilation...\n");

    // We will use a STATE MACHINE approach here
    set_compiler_state(START_COMPILATION);

    /**
     * The way this works is that we go step by step for the current file
     * The work on the current file will update the state of the compiler
     * Based on that state, the compilation keeps going
     */
    while (true)
    {
        switch (get_compiler_state())
        {
        case INVALID: // Error encountered
            log("Compiler Entered Invalid State\n");
            handle_invalid_state();
            break;
        case START_COMPILATION:
            log("Compiler Entered Compilation State\n");
            handle_start_compilation_state();
            break;
        case PARSE:
            log("Compiler Entered Parsing State\n");
            handle_parse_state();
            break;
        case DONE_COMPILING:
            goto _exit_;
        }
    }

_exit_:
    return true;
}

bool run_compiler()
{
}

void handle_start_compilation_state()
{
    // This is very simple.
    // We just start parsing
    set_compiler_state(PARSE);
}

void handle_parse_state()
{
    set_assumed_state(DONE_COMPILING); // set the next state first(for now 'done')
    file_context *current_fcont = get_current_file_context();
    if (!file_context_parse(current_fcont))
        return;
    // Do some post-parse things here
    set_assumed_state_as_compiler_state(); // make the next step the current step
}