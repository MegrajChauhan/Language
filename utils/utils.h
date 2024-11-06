#ifndef _UTILS_
#define _UTILS_

#include "report.h"
#include <stdlib.h>

// contains macro definitions only

#define crash() exit(EXIT_FAILURE)

#ifndef PTR_VALID
#define check_source(ptr)                                                              \
    do                                                                                 \
    {                                                                                  \
        if (!ptr)                                                                      \
        {                                                                              \
            fmt_log("PTR check failed in FILE %s, LINE %d\n", (__FILE__), (__LINE__)); \
            crash();                                                                   \
        }                                                                              \
    } while (0)
#define check_ptr(ptr, field)                                                          \
    do                                                                                 \
    {                                                                                  \
        if (!ptr || !ptr->field)                                                       \
        {                                                                              \
            fmt_log("PTR check failed in FILE %s, LINE %d\n", (__FILE__), (__LINE__)); \
            crash();                                                                   \
        }                                                                              \
    } while (0)
#else
#define check_source(ptr)
#define check_ptr(ptr, field)
#endif

#define IsSpace(c) (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define IsNum(c) ((c >= '0' && c <= '9'))
#define IsAlpha(c) ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#define IsAlnum(c) (IsNum(c) || IsAlpha(c))

// for identifiers as they may include 0-9, a-z, A-Z and _
#define IsStrInclusive(c) (IsAlnum(c) || c == '_')

#if defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define SEPARATOR '/'
#else
#define SEPARATOR '\\'
#endif

#define nil(ptr) (ptr = NULL)

#endif