#include <stdio.h>
#include "context.h"

int main()
{
    if (!gen_map())
    {
        printf("GEN MAP\n");
        return 0;
    }
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
    rid_of_map();
    return 0;
}