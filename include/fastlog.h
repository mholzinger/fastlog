#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>

/* protos */
void logpath(char* logfile);
void logf(const char* output, ...);
int openlogfile(char* logfilename);
void closelogfile(void);
void writesyslog(const char* format, ...);
char* getlogpath(void);

enum {TRUE = 1, FALSE = 0};
