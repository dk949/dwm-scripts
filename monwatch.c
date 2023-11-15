#include "common.h"

#include <libgen.h>
#include <string.h>
#include <sys/wait.h>

#ifndef MAXMONS
#    define MAXMONS 8
#endif
#ifndef MONWATCH_VERSION
#    define MONWATCH_VERSION "UNKNOWN"
#endif

typedef struct Mon {
    char const *path;
    char const *on_connect;
    char const *on_disconnect;
    char const *on_change;
    bool state;
} Mon;

_Static_assert(sizeof(Mon) == 40, "");

static Mon mons[MAXMONS] = {0};
static Mon *mons_end;

static struct timespec interval = {.tv_sec = 5, .tv_nsec = 0};
static char *progname;

static void readArgs(int argc, char **argv);
static void mainLoop(void);
static void updateAllStates(void);

int main(int argc, char **argv) {
    nice(19);
    readArgs(argc, argv);
    mainLoop();
}

/****************.
|      Main      |
`****************/


static bool getMonitorState(char const *path) {
    char c;
    FILE *fp = fopen(path, "r");
    if (!fp) DIE("Failed to open state path %s\n", path);
    if (fread(&c, 1, 1, fp) != 1) DIE("Failed to read state from path %s\n", path);
    fclose(fp);
    switch (c) {
        case 'c': return true;
        case 'd': return false;
        default: DIE("Expected state to be either connected or disconnected\n");
    }
}

static void mainLoop(void) {
    while (1) {
        for (Mon *it = mons; it != mons_end; it++) {
            bool state = getMonitorState(it->path);
            if (state != it->state) {
                if (it->on_change) runShell(it->on_change);
                if (state) {
                    if (it->on_connect) runShell(it->on_connect);
                } else {
                    if (it->on_disconnect) runShell(it->on_disconnect);
                }
                it->state = state;
            }
        }
        noInteruptSleep(interval);
    }
}

/****************.
|     Utils      |
`****************/

static void updateAllStates(void) {
    for (Mon *it = mons; it != mons_end; it++) {
        it->state = getMonitorState(it->path);
    }
}

/****************.
|      Args      |
`****************/

static void help(int code) {
    fprintf(code ? stderr : stdout,
        "Usage: %s [OPTIONS...] MONITOR_CONFIG...\n"
        "\n"
        "     MONITOR_CONFIG           A list of configurations for how to handle changes\n"
        "                              in monitor state up to %d configurations are\n"
        "                              supported.\n"
        "\n"
        "     A monitor config has the following syntax:\n"
        "       -- '/path/to/card/state' -c 'on_connect cmd' -d 'on_disconnect cmd' -C 'on_change cmd'\n"
        "\n"
        "     Notes:\n"
        "             Path has to be first, but -C, -c and -d can be in any order and/or\n"
        "             omitted.\n"
        "\n"
        "             All flags in a config can only appear once.\n"
        "\n"
        "             The callbacks will be executed in the order: -C then either -c or -d\n"
        "\n"
        "\n"
        "    --                        Monitor config separator\n"
        "    -C, --on-change CMD       Command to run when the monitor state changes\n"
        "    -c, --on-connect CMD      Command to run when the monitor state changes to `connected'\n"
        "    -d, --on-disconnect CMD   Command to run when the monitor state changes to `disconnected'\n"
        "\n"
        "    -i, --interval N          How much time to wait between checking state.\n"
        "                              As decimal seconds. Default: %li.0s.\n"
        "\n"
        "    -v, --version             Print progrm version and exit.\n"
        "    -h, --help                Print this message and exit.\n"
        "\n",
        progname,
        MAXMONS,
        interval.tv_sec);
    exit(code);
}

static void version(void) {
    printf("%s v" MONWATCH_VERSION "\n", progname);
    exit(0);
}

static void readArgs(int argc, char **argv) {
    progname = basename(argv[0]);
    if (argc <= 1) help(1);
    int curr_mon = 0;
#define is(x) (eq(argv[i], x))

    enum { BEGIN, MON } state = BEGIN;

    for (int i = 1; i < argc; i++) {
        if (is("-h") || is("--help")) help(0);
        if (is("-v") || is("--version")) version();
        switch (state) {
            case BEGIN:
                if (is("--")) {
                    if (++i >= argc) DIE("Expected a path after %s\n", argv[i - 1]);
                    mons[curr_mon].path = argv[i];
                    state = MON;
                } else if (is("-i") || is("--interval")) {
                    if (++i >= argc) DIE("Expected number after %s\n", argv[i - 1]);
                    double t = getFloat(argv[i], "Invalid decimal");
                    interval.tv_sec = t;
                    interval.tv_nsec = 1e9 * (t - (double)interval.tv_sec);
                } else
                    DIE("Unknown flag outside of a monitor configuration `%s'\n", argv[i]);
                break;
            case MON:
                if (is("-C") || is("--on-change")) {
                    if (++i >= argc) DIE("Expected a string after %s\n", argv[i - 1]);
                    if (mons[curr_mon].on_change) DIE("%s can only be specified once per config group", argv[i - 1]);
                    mons[curr_mon].on_change = argv[i];
                } else if (is("-c") || is("--on-connect")) {
                    if (++i >= argc) DIE("Expected a string after %s\n", argv[i - 1]);
                    if (mons[curr_mon].on_connect) DIE("%s can only be specified once per config group", argv[i - 1]);
                    mons[curr_mon].on_connect = argv[i];
                } else if (is("-d") || is("--on-disconnect")) {
                    if (++i >= argc) DIE("Expected a string after %s\n", argv[i - 1]);
                    if (mons[curr_mon].on_disconnect)
                        DIE("%s can only be specified once per config group", argv[i - 1]);
                    mons[curr_mon].on_disconnect = argv[i];
                } else if (is("--")) {
                    curr_mon++;
                    state = BEGIN;
                    i--;
                    if (curr_mon >= MAXMONS)
                        DIE("This version of %s cannot handle more than %d monitors\n", progname, MAXMONS);
                } else {
                    DIE("Unknown flag in monitor configuration `%s'\n", argv[i]);
                }
                break;
            default: DIE("Internal error unknown state %d\n", state);
        }
    }
    if (!mons[0].path) DIE("Specify at least one monitor to watch\n");
    mons_end = &mons[curr_mon + 1];
    updateAllStates();
#undef is
}
