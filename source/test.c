#include <stdio.h>
#include <stdlib.h>
#include "flog.h"

int main(void)
{
    logpath("/tmp/test_one.log");

    flog("The quick brown fox jumped over the lazy dog.");
    flog("int = <%d>, string = <%s>", (int)1234,
        (char*)"string with space characters");

    closelogfile();

    logpath("/var/log/syslog");

    flog("call to open syslog should fail, so this message should get "
        "written to syslog");
    closelogfile();

    logpath(NULL);
    flog("this should appear underneath an error statement in syslog");
    closelogfile();

    logpath("/tmp/test_two.log");
    flog("logerror was sucessfully unset message now appears in <%s>",
        getlogpath());
    closelogfile();

    flog("logpath is set <%s>, while the logf FILE* "
        "is NULL, message appears in syslog", getlogpath());

    return 0;
}
