#include <stdio.h>
#include "read.h"

int main(int argc, char **argv)
{
    file *f = file_read(argv[1]);
    // file *f = file_read("../temp.l");
    if (!f)
        return 0;
    while (stream_has_more(f->fdata))
    {
        // the following should allow us to access the entire buffer in case of string
        // but we will go character by character
        char *c = (char *)stream_read(f->fdata);
        putchar(*c);
    }
    file_close(f);
    return 0;
}