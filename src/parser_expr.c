#include "parser_expr.h"

ast *parse_ast(vec *expr)
{
    file_context *current = get_current_file_context();
    fmt_log("PARSING EXPRESSION: In Line %lu.\n", curr.line);
    ast *tree = (ast *)malloc(sizeof(ast));
    if (!tree)
    {
        fmt_internal_error("Failed to parse the expression at line %lu[FAILED TO ALLOCATE MEMORY].\n", curr.line);
        fmt_log("Failed to parse expression\n");
        return NULL;
    }

    if (!(tree->root = produce_new_node()))
    {
        report_internal_error("Failed to parse expressions.");
        fmt_log("Failed to parse expression\n");
        free(tree);
        return NULL;
    }

    // start parsing now
    // tree->root->node_token = curr;
    

    fmt_log("PARSING EXPRESSION(%sSUCCESS%s)", BOLDGREEN, RESET);
    return tree;
}

bool build_ast_from_root(ast *tree, vec *expr)
{
    check_ptr(tree, root);

    // expr contains all of the tokens that make up of the expression
    // For now, we will be preparing to parse array access as well
}