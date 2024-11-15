#include <stdio.h>
#include "lang.h"

int main(int argc, char **argv)
{
  if (!lang_main(argc, argv))
  {
    fmt_log("UNSUCCESSFUL COMPILATION TERMINATION...\n", NULL);
    report("Failure during compilation...\n", NULL);
    return -1;
  }
  return 0;
}