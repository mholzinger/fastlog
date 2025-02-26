#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flog.h"

#define TEST_LOG_1 "/tmp/test_one.log"
#define TEST_LOG_2 "/tmp/test_two.log"
#define SYSLOG_PATH "/var/log/syslog"

static void verify_log(const char* path, const char* expected);

void verify_log(const char* path, const char* expected) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        perror("Failed to open log file for verification");
        return;
    }

    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, expected) != NULL) {
            printf("Verification passed: Found expected output in %s\n", path);
        } else {
            printf("Verification failed: Expected output not found in %s\n", path);
        }
    } else {
        printf("Verification failed: Log file is empty or unreadable\n");
    }

    fclose(fp);
}

int main(void)
{
    int tests_passed = 0;
    int tests_failed = 0;

    // Test 1: Basic log output
    printf("Testing log output to %s...\n", TEST_LOG_1);
    if (logpath(TEST_LOG_1) == NO_ERROR) {
        flog("The quick brown fox jumped over the lazy dog.");
        flog("int = <%d>, string = <%s>", (int)1234, "string with space characters");
        verify_log(TEST_LOG_1, "The quick brown fox");
        tests_passed++;
    } else {
        fprintf(stderr, "Failed to set log path to %s\n", TEST_LOG_1);
        tests_failed++;
    }
    closelogfile();

    // Test 2: Invalid log path (should write to syslog)
    printf("Testing invalid log path...\n");
    if (logpath("/invalid/path/to/logfile.log") == LOG_ERROR) {
        flog("This message should appear in syslog.");
        printf("Test passed: logerror set correctly for invalid path\n");
        tests_passed++;
    } else {
        printf("Test failed: logerror not set for invalid path\n");
        tests_failed++;
    }
    closelogfile();

    // Test 3: NULL log path (should write to syslog)
    printf("Testing NULL log path...\n");
    logpath(NULL);
    flog("This should appear underneath an error statement in syslog.");
    closelogfile();

    // Test 4: Reset logerror and log to a new file
    printf("Testing logerror reset...\n");
    if (logpath(TEST_LOG_2) == NO_ERROR) {
        flog("logerror was successfully unset. Message now appears in %s", TEST_LOG_2);
        verify_log(TEST_LOG_2, "logerror was successfully unset");
        tests_passed++;
    } else {
        fprintf(stderr, "Failed to set log path to %s\n", TEST_LOG_2);
        tests_failed++;
    }
    closelogfile();

    // Test 5: Log with NULL FILE* (should write to syslog)
    printf("Testing log with NULL FILE*...\n");
    flog("logpath is set to %s, but the logfp FILE* is NULL. Message appears in syslog.", getlogpath());

    // Clean up
    remove(TEST_LOG_1);
    remove(TEST_LOG_2);

    // Print summary
    printf("\nTest summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);

    return (tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

