#include "slice.h"

const char *slice_to_cstr(slice *s)
{
    // 's' cannot be NULL
    size_t len = (size_t)(s->_e - s->_s) + 1;
    char *str = malloc(len);
    if (str == NULL)
        return NULL;
    memcpy(str, s->_s, len - 1);
    str[len - 1] = 0;
    return (const char *)str;
}