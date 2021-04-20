#include <stdarg.h>

extern char * trc;
extern FILE * trace;

void trace_func(const char *format, ... ){
    va_list  ap;
    va_start(ap, format);
    if (trc == 0)
    {
        vprintf(format, ap);
    } else
    {
        vfprintf(trace,format, ap);
    }

    va_end(ap);
}