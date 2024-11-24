#include "common_hdlr.h"

void print_filename_and_details(slice *filename, size_t col, size_t line, char *msg)
{
    slice_print(filename, BOLDWHITE);
    printf("%s:%lu:%lu:%s %s\n", BOLDWHITE, line, col, RESET, msg);
}

void print_line_with_highlight(char *st, char *highlight_st, char *highlight_ed)
{
    while (st != highlight_st)
    {
        putchar(*st);
        st++;
    }
    printf("%s", BOLDRED);
    while (st != highlight_ed)
    {
        putchar(*st);
        st++;
    }
    printf("%s\n", RESET);
    while (*st != '\n' && *st != '\0')
    {
        putchar(*st);
        st++;
    }
}