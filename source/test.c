#include <stdio.h>
#include <stdlib.h>
#include "fastlog.h"

int main()
{
    logpath("/tmp/test_one.log");

    logf("The quick brown fox jumped over the lazy dog.\n");
    logf("int = <%d>, string = <%s>\n", (int)1234, 
        (char*)"string with space characters");

    closelogfile();

    logpath("/var/log/syslog");

    logf("call to open syslog should fail, so this message should get "
        "written to syslog");
    closelogfile();

    logpath(NULL);
    logf("this should appear underneath an error statement in syslog");
    closelogfile();

    logpath("/tmp/test_two.log");
    logf("logerror was sucessfully unset message now appears in <%s>\n", 
        getlogpath());
    closelogfile();

    logf("logpath is set <%s>, while the logf FILE* "
        "is NULL, message appears in syslog", getlogpath());

    return 0;
}
