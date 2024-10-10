#include <stdio.h>
#include "context.h"

int main()
{
    char name[] = "examples/temp.l";
    context *c = create_context(name);
    if (!c)
    {
        printf("CREATE CONTEXT\n");
        return 0;
    }
    if (!gen_file_context_tree(c->fcont, c->_included_files))
    {
        printf("GEN TREE\n");
    }
    destroy_context(c);
    return 0;
}