#ifndef _REPORT_
#define _REPORT_

#include <stdio.h>

#define report(msg, ...) fprintf(stderr, msg, __VA_ARGS__)

#define _internal_error() report("INTERNAL ERROR.\n", NULL)
#define internal_error() _internal_error() // if there is no printable reason of the error

#define _report_internal_error(CAUSE) report("INTERNAL ERROR: %s\n", CAUSE)
#define report_internal_error(cause) _report_internal_error(cause) // if we can present the issue

#define _fmt_internal_error(fmt, ...) report("INTERNAL ERROR:" fmt, __VA_ARGS__)
#define fmt_internal_error(fmt, ...) _fmt_internal_error(fmt, __VA_ARGS__) // if there is more details to be provided

#ifndef NO_LOG_STEPS
#define _log(msg) report(msg, NULL)
#define log(msg) _log(msg)

#define fmt_log(fmt, ...) report(fmt, __VA_ARGS__)
#else
#define log(msg)
#define fmt_log(fmt, ...)
#endif

#define COMPILER_DEBUG /// TODO: Remove this

#ifdef COMPILER_DEBUG
#define assert(cond) do{if (!(cond)){fmt_log("ASSERTION FAILED: The condition "#cond" failed at line: %lu and file: %s", __LINE__, __FILE__)}}while(0);
#endif

#endif