#include "slice.h"

slice *slice_create(char *st, char *ed)
{
    slice *sl = (slice *)malloc(sizeof(slice));
    if (!sl)
    {
        report_internal_error("Failed to allocate a slice");
        return NULL;
    }
    sl->ed = ed;
    sl->st = st;
    return sl;
}

inline size_t slice_len(slice *sl)
{
    return ((sl->ed - sl->st));
}

char *slice_to_str(slice *sl)
{
    size_t len = slice_len(sl) + 1;
    char *str = (char *)malloc(len);
    if (!str)
    {
        report_internal_error("Failed to convert slice to string.");
        return NULL;
    }
    memcpy(str, sl->st, len - 1);
    str[len] = 0;
    return str;
}

bool slice_cmp(void *sl1, void *sl2)
{
    if (slice_len((slice*)sl1) != slice_len((slice*)sl2))
        return false;
    char *iter1 = ((slice*)sl1)->st;
    char *iter2 = ((slice*)sl2)->st;
    while (iter1 != ((slice*)sl2)->ed)
    {
        if (*iter1 != *iter2)
            return false;
        iter1++;
        iter2++;
    }
    return true;
}

void slice_destroy(void *sl)
{
    check_source(sl);
    ((slice*)sl)->ed = NULL;
    ((slice*)sl)->st = NULL;
    free(sl);
}

size_t slice_hash(void *sl, size_t bucket_count)
{
    char *temp = slice_to_str((slice*)sl);
    if (!temp)
    {
        report_internal_error("Failed to hash a slice.");
        crash(); // don't continue here
    }
    size_t ret = string_hash(temp, bucket_count);
    free(temp);
    return ret;
}