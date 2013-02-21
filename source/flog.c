#include "fastlog.h"

/* globals */
int logerror = 0;
char* logfilename = NULL;
FILE* logfp = NULL;

/*---------------------------------------------------------------------------*/
char* getlogpath(void)
{
    return logfilename;
}

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
    va_start(args, output);
    vsprintf(logstring, output, args);
    va_end(args);

    /* If logerror is set or no FILE* is open, write to syslog instead */
    if ((logerror == TRUE) || (logfp == NULL))
    {
        writesyslog(logstring);
        return;
    }

    (void)fwrite(logstring, 1, strlen(logstring), logfp);

}

/*---------------------------------------------------------------------------*/
int openlogfile(char* logfilename)
{
    if (logfilename == NULL)
    {
        writesyslog("openlogfile err: logfilename is unset (null)");
        logerror = TRUE;
        return logerror;
    }

    logfp = fopen((const char*)logfilename, (const char*)"wb");
    if (logfp == NULL)
    {
        writesyslog("openlogfile err: fopen failed for logfilename <%s>",
        logfilename);
        logerror = TRUE;
        return logerror;
    }

    logerror =  FALSE;
    return logerror;
}

/*---------------------------------------------------------------------------*/
void closelogfile(void)
{
    if (logfp != NULL) (void)fclose(logfp);
    logfp = NULL;
}

/*---------------------------------------------------------------------------*/
void writesyslog (const char* format, ...)
{
     char logstring[1024];

     va_list args;
     va_start (args, format);
     vsprintf (logstring, format, args);
     va_end (args);

     setlogmask (LOG_UPTO (LOG_NOTICE));
     openlog("fastlog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
     syslog(LOG_NOTICE, logstring, getuid ());
     closelog();
}

/*---------------------------------------------------------------------------*/
