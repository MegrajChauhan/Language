#include "symtable.h"

symtable *symtable_init()
{
    symtable *s = (symtable *)malloc(sizeof(symtable));
    if (s == NULL)
        return NULL;
    s->entries = umap_init(20);
    if (!s->entries)
    {
        free(s);
        return NULL;
    }
    return s;
}

void symtable_destroy(symtable *table)
{
    umap_entry *e = table->entries->buckets;
    for (size_t i = 0; i < table->entries->bucket_count; i++)
    {
        if (e[i].key != NULL)
        {
            umap_entry *curr = e[i].next;
            while (curr != NULL)
            {
                umap_entry *tmp = curr->next;
                symtable_entry *sym = (symtable_entry *)curr->value;
                free(sym);
                curr = tmp;
            }
        }
    }
    umap_destroy(table->entries);
    free(table);
}

bool symtable_find(symtable *table, slice name)
{
    return umap_find(table->entries, &name) != NULL;
}

symtable_entry *symtable_get(symtable *table, slice name)
{
    umap_entry *e = umap_find(table->entries, &name);
    return e? (symtable_entry *)(e->value): NULL;
}

bool symtable_add(symtable *table, symtable_entry *entry, slice name)
{
    // we won't check if an entry already exists
    // this is because we confirm that first before we even add
    symtable_entry *ent = (symtable_entry *)malloc(sizeof(symtable_entry));
    if (!ent)
    {
        internal_err();
        return false;
    }
    memcpy(ent, entry, sizeof(symtable_entry));
    if (!umap_insert_slice(table->entries, &name, (uint64_t)ent))
    {
        internal_err();
        return false;
    }
    return true;
}