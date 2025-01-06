#include "global_context.h"

bool populate_keys()
{
    fmt_log("POPULATING KEYS....\n", NULL);
    keys = umap_create(KEYWORD_COUNT /*This doesn't necessarily mean the number of keywords but the number of buckets*/, slice_hash, slice_cmp, NULL, NULL);
    if (!keys)
    {
        report_internal_error("Failed to allocate memory for keys.");
        return false;
    }
    for (size_t i = 0; i < sizeof(_keys) / sizeof(slice); i++)
    {
        register char *_k = _key_values_[i];
        register size_t len = strlen(_k);
        _keys[i].st = _k;
        _keys[i].ed = _k + len;
        if (!umap_insert(keys, (void *)(&_keys[i]), (void *)((uintptr_t)(TOK_FIRST_KEY + 1 + i))))
        {
            fmt_internal_error("Failed to create pair for key %s.\n", _k);
            umap_destroy(keys);
            nil(keys);
            return false;
        }
        fmt_log("ADDED KEY: %s\n", _k);
    }
    return true;
}

token_t is_a_key(slice *val)
{
    token_t tok;
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
