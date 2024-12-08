#include "lexer_hdlr.h"

define_errhdlr(__invalid_floating_point_number)
{
    lexer *l = (lexer *)comp;
    lexer_state *ls = (lexer_state *)state;
    file_context *cont = get_current_file_context();
    print_filename_and_details(cont->file_name, ls->col, ls->line, "Invalid floating point number found.");
    print_line_with_highlight(ls->val.st - ls->col, ls->val.st, ls->val.ed);
    return true;
}

define_errhdlr(__invalid_number_base_type)
{
    lexer *l = (lexer *)comp;
    lexer_state *ls = (lexer_state *)state;
    file_context *cont = get_current_file_context();
    print_filename_and_details(cont->file_name, ls->col, ls->line, "The given prefix suggests one number format while a different format was given");   
    print_line_with_highlight(ls->val.st - ls->col, ls->val.st, ls->val.ed);
    return true;
}

define_errhdlr(__unknown_token)
{
    lexer *l = (lexer *)comp;
    lexer_state *ls = (lexer_state *)state;
    file_context *cont = get_current_file_context();
    print_filename_and_details(cont->file_name, ls->col, ls->line, "Cannot build a token from this. Expected a NUMBER, IDENTIFIER, OPERATOR or KEYWORD");   
    print_line_with_highlight(ls->val.st - ls->col, ls->val.st, ls->val.st+1);
    return true;
}