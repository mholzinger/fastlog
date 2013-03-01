#ifndef _FLOG
#define _FLOG

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

enum flog_bool {LOG_ERROR = 1, NO_ERROR = 0};

/* protos */
void logpath(char* logfile);
void flog(const char* output, ...);
int openlogfile(char* logfilename);
void closelogfile(void);
void writesyslog(const char* format, ...);
char* getlogpath(void);

#endif /*_FLOG*/
