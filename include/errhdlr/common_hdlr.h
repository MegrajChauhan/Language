#ifndef _COMMON_HDLR_
#define _COMMON_HDLR_

#include "slice.h"
#include "utils.h"
#include "report.h"
#include <stdio.h>

#define define_errhdlr(name) bool name(void *state, void *comp)

// Define how the lines should be printed
static bool _print_tab = false;

void set_printer_state(bool put_tab);

void print_filename_and_details(slice *filename, size_t col, size_t line, char *msg);

void print_line_with_highlight(char *st, char *highlight_st, char *highlight_ed);

#endif