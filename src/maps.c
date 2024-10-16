#include "maps.h"

bool gen_map()
{
    key_map = umap_init(20); // 20 ought to do!
    if (!key_map)
        return false;
    oper_map = umap_init(10); // 10 should do
    if (!oper_map)
    {
        umap_destroy(key_map);
        return false;
    }
    // we will manually enter every single entry now
    // in case the language gets big someday with many keywords
    // and operators then this will be a serious bottleneck
    char *keys[] =
        {
            "u8",
            "u16",
            "u32",
            "u64",
            "i8",
            "i16",
            "i32",
            "i64",
            "f32",
            "f64", // no f128 since the VM doesn't support
            "void",
            "bool",
            "true",
            "false",

            "include",
            "assembly",

            "if",
            "else",

            "var",
            "const",
            "global",
            "fn",

            "enum",
            "struct",
        };
    char *opers[] =
        {
            ":",
            ";",
            ",",
            "=",

            "#",
            "$",

            "(",
            ")",
            "{",
            "}",
            "[",
            "]",
            "\"",
            "'",
            ".",

            "==",
        };
    size_t i = 0;
    for (; i < 24; i++)
    {
        if (!umap_insert(key_map, keys[i], i))
            return false;
    }
    for (size_t j = 0; j < 16; j++, i++)
    {
        if (!umap_insert(oper_map, opers[j], i))
            return false;
    }
    return true;
}

bool find_key(slice *s, uint64_t *_s)
{
    umap_entry *res = umap_find(key_map, s);
    if (!res)
        return false;
    *_s = res->value;
    return true;
}

bool find_oper(slice *s, uint64_t *_s)
{
    umap_entry *res = umap_find(oper_map, s);
    if (!res)
        return false;
    *_s = res->value;
    return true;
}

void rid_of_map()
{
    umap_destroy(key_map);
    umap_destroy(oper_map);
}