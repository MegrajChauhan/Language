#include "lang.h"

bool lang_main(int argc, char **argv)
{
    // We need to perform more stuff here
    // but before that, we do other stuff here

    /// TODO: We need argument parsing set up
    /// TODO: We need other compiler initializations to do which will come as they are implemented

    fmt_log("Initializing the COMPILER...\n", NULL);
    if (!populate_keys())
    {
        report_internal_error("KEYS could not be populated.");
        return false;
    }
    if (!create_compilation_context())
    {
        report_internal_error("Failed to create a compiler context.");
        return false;
    }
    // For now, argv[1] will have the file name
    char *inp_file = argv[1];

    fmt_log("Creating PARENT file context[Input File: '%s']\n", inp_file);
    file_context *inp_cont = file_context_create(NULL);
    if (!inp_cont)
    {
        report_internal_error("Failed to create the base file context.");
        goto _err;
    }
    slice *input = slice_create(inp_file, inp_file + strlen(inp_file));
    if (!input)
        goto _err;
    if (!file_context_populate_stream(inp_cont, input))
        goto _err;
    if (!add_base_file_context(inp_cont))
    {
        report_internal_error("Failed to setup the BASE file context.");
        goto _err;
    }
    fmt_log("Successfully setup the base file context\n", NULL);
    if (!compile())
    {
        report_internal_error("Failed to start compilation.");
        goto _err;
    }

    destroy_compilation_context();
    destroy_keys();
    return true;
_err:
    destroy_compilation_context();
    destroy_keys();
    return false;
}

void help()
{
    printf("%s",
           "Usage: lang [OPTIONS]... <Input file>\n"
           "-h, -help                       --> Print this help\n"
           "-v, -version                    --> Get the compiler version\n");
}

void version()
{
    printf("%s\n",
           "The lang Programming Language\n"
           "Version 0.0.0\n");
}