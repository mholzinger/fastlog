#ifndef _FLOG
#define _FLOG

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

enum flog_bool {LOG_ERROR = 1, NO_ERROR = 0};

/* protos */
void closelogfile(void);
void flog(const char* output, ...);
void flushlog(void);
char* getlogpath(void);
void logpath(char* logfile);
int openlogfile(char* logfilename);
void writesyslog(const char* format, ...);
#endif /*_FLOG*/
