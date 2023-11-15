#ifndef COMMON_H
#define COMMON_H

#include <errno.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

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

bool eq(char const *a, char const *b){
    return !strcmp(a, b);
}



#endif  // COMMON_H
