#include "global_context.h"

bool populate_keys()
{
    return (keys = umap_create(KEYWORD_COUNT, slice_hash_str, slice_cmp, slice_destroy, NULL)) != NULL;
}

token_t is_a_key(slice *val)
{
    token_t tok = (token_t)umap_find(keys, val);
    void *type = umap_find(keys, val);
    if (type)
        tok = (token_t)type;
    else
        tok = TOK_IDENTIFIER;
    return tok;
}

void destroy_keys()
{
    umap_destroy(keys);
    keys = NULL;
}