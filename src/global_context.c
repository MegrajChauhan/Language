#include "global_context.h"

bool populate_keys()
{
    return (keys = umap_create(KEYWORD_COUNT /*This doesn't necessarily mean the number of keywords but the number of buckets*/, slice_hash_str, slice_cmp, slice_destroy, NULL)) != NULL;
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

void set_compiler_state(compilerState_t state)
{
    cstate = state;
}

compilerState_t get_compiler_state()
{
    return cstate;
}

void set_assumed_state(compilerState_t state)
{
    assumed_next_state = state;
}

void set_assumed_state_as_compiler_state()
{
    cstate = assumed_next_state;
}
