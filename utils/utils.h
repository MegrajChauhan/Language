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
#define IsIdenNameInclusive(c) (IsAlnum(c) || c == '_')

#if defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
#define SEPARATOR '/'
#else
#define SEPARATOR '\\'
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h> // For Windows _getcwd
#define gcd _getcwd
#else
#include <unistd.h> // For Unix/Linux getcwd
#define gcd getcwd
#endif

#define nil(ptr) (ptr = NULL)

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BOLDBLACK "\033[1;30m"
#define BOLDRED "\033[1;31m"
#define BOLDGREEN "\033[1;32m"
#define BOLDYELLOW "\033[1;33m"
#define BOLDBLUE "\033[1;34m"
#define BOLDMAGENTA "\033[1;35m"
#define BOLDCYAN "\033[1;36m"
#define BOLDWHITE "\033[1;37m"

#define UNDERLINE "\033[4m"
#define BOLD "\033[1m"

#define ERROR "ERROR"
#define WARNING "WARNING"

#endif