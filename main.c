#include <stdio.h>
#include "context.h"

int main()
{
    if (!gen_map())
    {
        printf("GEN MAP\n");
        return 0;
    }
    char name[] = "examples/parsetest.l";
    context *c = create_context(name);
    if (!c)
    {
        printf("CREATE CONTEXT\n");
        return 0;
    }
    if (!context_make_first_pass(c))
    {
        printf("FIRST PASS\n");
        return 0;
    }
    context_parse_file_tree(c);
    destroy_context(c);
    rid_of_map();
    return 0;
}