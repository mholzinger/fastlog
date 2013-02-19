#include <stdio.h>
#include <stdlib.h>
#include "fastlog.h"

int main()
{
    int idx = 9;
    char* str = "pattern with space chars";

    logpath("/tmp/test.log");

    logf("The quick brown fox jumped over the lazy dog.\n");
    logf("int = <%d>, string = <%s>\n", idx, str);

    closelogfile();

    return 0;
}
