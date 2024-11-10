#include "slice.h"

slice *slice_create(char *st, char *ed)
{
    slice *sl = (slice *)malloc(sizeof(slice));
    if (!sl)
    {
        report_internal_error("Failed to allocate a slice");
        return NULL;
    }
    // The use of this function should generally mean this
    size_t len = ed - st;
    sl->_str = (char *)malloc(len + 1);
    if (!sl->_str)
    {
        report_internal_error("Failed to populate a slice");
        free(sl);
        return NULL;
    }
    memcpy(sl->_str, st, len);
    sl->_str[len] = 0;

    // Since 'st' should be pointing to the memory we just allocated
    // Let's update 'ed' for the ease of finding length and other things
    sl->ed = &sl->_str[len];
    return sl;
}

// Useful in any context
inline size_t slice_len(slice *sl)
{
    return ((sl->ed - sl->st));
}

// This function used in the context when we haven't created a string
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
    str[len - 1] = 0;
    return str;
}

// For both contexts
bool slice_cmp(void *sl1, void *sl2)
{
    if (slice_len((slice *)sl1) != slice_len((slice *)sl2))
        return false;
    char *iter1 = ((slice *)sl1)->st;
    char *iter2 = ((slice *)sl2)->st;
    while (iter1 != ((slice *)sl2)->ed)
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
    if (((slice *)sl)->_str)
        free(((slice *)sl)->_str);
    ((slice *)sl)->ed = NULL;
    ((slice *)sl)->st = NULL;
    free(sl);
}

// for single contexts
size_t slice_hash(void *sl, size_t bucket_count)
{
    char *temp = slice_to_str((slice *)sl);
    if (!temp)
    {
        report_internal_error("Failed to hash a slice.");
        crash(); // don't continue here
    }
    size_t ret = string_hash(temp, bucket_count);
    free(temp);
    return ret;
}

// for string contexts
size_t slice_hash_str(void *sl, size_t bucket_count)
{
   return string_hash(((slice*)sl)->_str, bucket_count);
}