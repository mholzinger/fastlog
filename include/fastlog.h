#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* protos */
void logpath(char* logfile);
void logf(const char* output, ...);
int openlogfile(char* logfilename);
void closelogfile(void);
