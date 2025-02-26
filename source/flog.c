#include "flog.h"

/* globals */
int logerror = 0;
char* logfilename = NULL;
FILE* logfp = NULL;

/*---------------------------------------------------------------------------*/
void closelogfile(void)
{
    if (logfp != NULL) (void)fclose(logfp);
    logfp = NULL;
}

/*----------------------------------------------------------------------------*/
void flog(const char* output, ...)
{
    char logstring[1024];
    size_t array_count; // Change to size_t

    va_list args;
    va_start(args, output);
    vsnprintf(logstring, sizeof(logstring), output, args);
    va_end(args);

    /* If logerror is set or no FILE* is open, write to syslog instead */
    if ((logerror == LOG_ERROR) || (logfp == NULL))
    {
        writesyslog(logstring);
        return;
    }

    /* Append newline char */
    array_count = strlen(logstring); // strlen returns size_t
    if (array_count < sizeof(logstring) - 1) // Now both are size_t
    {
        logstring[array_count++] = 0x0a;
        logstring[array_count] = 0x00;
    }

    if (fwrite(logstring, 1, strlen(logstring), logfp) != strlen(logstring))
    {
        writesyslog("flog fwrite failed for log file <%s>\n", getlogpath());
        logerror = LOG_ERROR;
    }

    flushlog();
}


/*----------------------------------------------------------------------------*/
void flushlog(void)
{
    int errno_state;
    int irc = 0; // Initialize to 0 (success)

    errno = 0;
    if (logfp != NULL) irc = fflush(logfp); // Update irc only if logfp is not NULL
    if (irc) // Now irc is always initialized
    {
        errno_state = errno;
        writesyslog(
            "flushlog fflush failed for log file <%s>\n", getlogpath());
        writesyslog("errno <%d>\n", errno_state);
        logerror = LOG_ERROR;
    }
}


/*----------------------------------------------------------------------------*/
char* getlogpath(void)
{
    return logfilename;
}

/*----------------------------------------------------------------------------*/
int logpath(char* logfile)
{
    logfilename = logfile;
    return openlogfile(logfilename);
}

/*----------------------------------------------------------------------------*/
int openlogfile(char* logfilename)
{
    if (logfilename == NULL)
    {
        writesyslog("openlogfile err: logfilename is unset (null)");
        logerror = LOG_ERROR;
        return logerror;
    }

    logfp = fopen((const char*)logfilename, (const char*)"wb");
    if (logfp == NULL)
    {
        writesyslog("openlogfile err: fopen failed for logfilename <%s>",
            logfilename);
        logerror = LOG_ERROR;
        return logerror;
    }

    logerror = NO_ERROR;
    return logerror;
}

/*----------------------------------------------------------------------------*/
void writesyslog(const char* format, ...)
{
     char logstring[1024];

     va_list args;
     va_start(args, format);
     vsprintf(logstring, format, args);
     va_end(args);

     setlogmask(LOG_UPTO (LOG_NOTICE));
     openlog("libflog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
     syslog(LOG_NOTICE, logstring, getuid());
     closelog();
}

/*---------------------------------------------------------------------------*/
