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