#include "fastlog.h"

/* globals */
char* logfilename = NULL;
FILE* logfp = NULL;

/*---------------------------------------------------------------------------*/
void logpath(char* logfile)
{
    logfilename = logfile;
    openlogfile(logfilename);
}

/*---------------------------------------------------------------------------*/
void logf(const char* output, ...)
{
    char logstring[1024];

    va_list args;
    va_start (args, output);
    vsprintf (logstring, output, args);
    va_end (args);

    (void)fwrite(logstring, 1, strlen(logstring), logfp);

}

/*---------------------------------------------------------------------------*/
int openlogfile(char* logfilename)
{
    if (logfilename == NULL)
        return 1;

    logfp = fopen((const char*)logfilename, (const char*)"wb");
    if (logfp == NULL)
        return 1;

    return 0;
}

/*---------------------------------------------------------------------------*/
void closelogfile(void)
{
    (void)fclose(logfp);
}

/*---------------------------------------------------------------------------*/
