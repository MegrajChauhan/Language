#ifndef _UTILS_
#define _UTILS_

#include "report.h"
#include <stdlib.h>

#define crash() exit(EXIT_FAILURE)

#ifndef PTR_VALID
#define check_ptr(ptr, field)                                \
    do                                                       \
    {                                                        \
        if (!ptr || !ptr->field)                             \
        {                                                    \
            fmt_log("PTR check failed in FILE %s, LINE %d\n", (__FILE__), (__LINE__)); \
            crash();                                         \
        }                                                    \
    } while (0)
#else
#define check_ptr(ptr, field)
#endif

#endif