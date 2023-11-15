#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define DIE(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    } while (0)
#define WARN(...)                              \
    do                                         \
        fprintf(stderr, "WARN: " __VA_ARGS__); \
    while (0)

static void noInteruptSleep(struct timespec ts) {
    static struct timespec remain;
    remain = ts;
    while (nanosleep(&remain, &remain) == -1 && errno == EINTR) { }
}

static bool eq(char const *a, char const *b) {
    return !strcmp(a, b);
}

static long getInt(char const *str, char const *msg) {
    char *end;
    long ret = strtol(str, &end, 0);
    if (*end) DIE("%s: `%s'\n", msg, str);
    return ret;
}

static double getFloat(char const *str, char const *msg) {
    char *end;
    double ret = strtod(str, &end);
    if (*end) DIE("%s: `%s'\n", msg, str);
    return ret;
}

static int runCmd(char const *pname, char *const args[]) {
    switch (vfork()) {
        case -1: DIE("Failed to fork the process: %s\n", strerror(errno));
        case 0: execvp(pname, args); DIE("Failed to run notify-send: %s\n", strerror(errno));
        default: {
            int status;
            wait(&status);
            if (WIFEXITED(status)) {
                if ((status = WEXITSTATUS(status))) WARN("Child process failed with status %d\n", status);
            } else if (WIFSIGNALED(status))
                WARN("Child process was killed with signal %d\n", WTERMSIG(status));
            else if (WIFSTOPPED(status))
                WARN("Child process was stopped with signal %d\n", WSTOPSIG(status));
            else
                DIE("Child process exited unexpectedly: %s\n", strerror(errno));
            return status;
        }
    }
}

// Just system for now, but might change later
static int runShell(char const *command) {
    return system(command);
}

#endif  // COMMON_H
