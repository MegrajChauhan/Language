#include "lexer_hdlr.h"

define_errhdlr(__invalid_floating_point_number)
{
    lexer *l = (lexer *)comp;
    lexer_state *ls = (lexer_state *)state;
    file_context *cont = get_current_file_context();
    print_filename_and_details(cont->file_name, ls->col, ls->line, "Invalid floating point number found.");
    
    // We need to print until a new-line here
    char *st = ls->val.st - ls->col;
    while (st != ls->val.st)
    {
        putchar(*st);
        st++;
    }
    printf("%s", BOLDRED);
    while (st != ls->val.ed)
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

define_errhdlr(__invalid_number_base_type)
{
    lexer *l = (lexer *)comp;
    lexer_state *ls = (lexer_state *)state;
    file_context *cont = get_current_file_context();
    print_filename_and_details(cont->file_name, ls->col, ls->line, "The given prefix suggests one number format while a different format was given");
    
    // We need to print until a new-line here
    char *st = ls->val.st - ls->col;
    while (st != ls->val.st)
    {
        putchar(*st);
        st++;
    }
    printf("%s", BOLDRED);
    while (st != ls->val.ed)
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