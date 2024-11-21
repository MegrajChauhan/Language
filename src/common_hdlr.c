#include "common_hdlr.h"

void print_filename_and_details(slice *filename, size_t col, size_t line, char *msg)
{
    slice_print(filename, BOLDWHITE);
    printf("%s:%lu:%lu:%s %s\n", BOLDWHITE, line, col, RESET, msg);
}