#include "context.h"

static void context_clear_namespace(namespace *ns)
{
    for (size_t i = 0; i < ns->children->count; i++)
    {
        namespace *x = vec_at(ns->children, i);
        context_clear_namespace(x);
    }
    // No worries about node
    namespace_destroy(ns);
}

context *create_context(char *input_fname)
{
    context *c = (context *)malloc(sizeof(context));
    if (c == NULL)
        return NULL;
    c->errors = error_init();
    if (c->errors == NULL)
    {
        free(c);
        return NULL;
    }
    c->fcont = (file_context *)malloc(sizeof(file_context));
    if (c->fcont == NULL)
    {
        error_destroy(c->errors);
        free(c);
        return NULL;
    }
    c->_included_files = uset_init(20);
    if (!c->_included_files)
    {
        error_destroy(c->errors);
        free(c->fcont);
        free(c);
        return NULL;
    }
    if (!init_file_context(c->fcont, input_fname))
    {
        uset_destroy(c->_included_files);
        error_destroy(c->errors);
        free(c->fcont);
        free(c);
        return NULL;
    }
    if (!(c->ns = namespace_init(NULL)))
    {
        destroy_file_context(c->fcont);
        uset_destroy(c->_included_files);
        error_destroy(c->errors);
        free(c->fcont);
        free(c);
        return NULL;
    }
    return c;
}

void destroy_context(context *c)
{
    context_clear_namespace(c->ns);
    destroy_file_context(c->fcont);
    uset_destroy(c->_included_files);
    error_destroy(c->errors);
    free(c);
}

bool context_make_first_pass(context *c)
{
    if (!gen_file_context_tree(c->fcont, c->_included_files))
        return false;
    return true;
}

bool context_parse_file_tree(context *c)
{
    parser *p = parser_init(c->fcont, c->errors, c->ns);
    if (!p)
    {
        internal_err();
        return false;
    }
    if (!parse(p))
        error_evaluate(c->errors);
    parser_destroy(p);
    return true;
}
