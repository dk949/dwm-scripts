#include "common.h"

#include <libgen.h>
#include <string.h>
#include <sys/wait.h>

#ifndef BATNOTIFY_VERSION
#    define BATNOTIFY_VERSION "UNKNOWN"
#endif

static char const *BAT_PATH = NULL;
static int urgent_level = 5;
static int warning_level = -1;
static int urgent_sent = 0, warning_sent = 0;
static char *urgent_icon = "battery-caution";
static char *warning_icon = "battery-low";
static char *urgent_header = "Battery is critically low";
static char *warning_header = "Battery is low";
static char urgent_body[128];
static char warning_body[128];

static struct timespec interval = {.tv_sec = 5, .tv_nsec = 0};
static char *progname;

static void readArgs(int argc, char **argv);
static void mainLoop(void);
static void notifySend(char *urgency, char *icon, char *head, char *body);

int main(int argc, char **argv) {
    readArgs(argc, argv);
    mainLoop();
}

/****************.
|      Main      |
`****************/


static int getBatteryLevel(void) {
#define SZ 256
    static char buf[SZ];

    FILE *bat = fopen(BAT_PATH, "r");
    if (!bat) DIE("Could not open capacity file `%s': %s\n", BAT_PATH, strerror(errno));
    errno = 0;
    size_t read = fread(buf, 1, SZ - 1, bat) + 1;
    fclose(bat);

    if (errno) DIE("An error occured when reading capacity file `%s': %s\n", BAT_PATH, strerror(errno));
    if (read == 0 || read >= SZ) DIE("Unexpected number of characters read from buffer file (%d)\n", SZ - 1);

    buf[read] = 0;
    while (read-- > 0)
        if (buf[read] == '\n') buf[read] = 0;

    return getInt(buf, "Value obtained from capacity file is not an integer\n");
#undef SZ
}

static void urgentSend(void) {
    if (urgent_sent) return;
    notifySend("critical", urgent_icon, urgent_header, urgent_body);
    urgent_sent = 1;
}

static void warningSend(void) {
    if (warning_sent) return;
    notifySend("normal", warning_icon, warning_header, warning_body);
    warning_sent = 1;
}

static void clearAll(void) {
    urgent_sent = 0;
    warning_sent = 0;
}

static void mainLoop(void) {
    while (1) {
        long l = getBatteryLevel();
        if (l <= urgent_level)
            urgentSend();
        else if (l <= warning_level)
            warningSend();
        else
            clearAll();

        noInteruptSleep(interval);
    }
}

/****************.
|      Utils     |
`****************/

static void notifySend(char *urgency, char *icon, char *head, char *body) {
    enum { PROGNAME = 2, URGENCY = 4, HEAD = 5, BODY = 6, ICON = 8, END };

    static char *args[] = {
        [0] = "notify-send",
        [PROGNAME - 1] = "-a",
        [URGENCY - 1] = "-u",
        [ICON - 1] = "-i",
        [END] = NULL,
    };
    args[PROGNAME] = progname;
    args[URGENCY] = urgency;
    args[HEAD] = head;
    args[BODY] = body;
    if (icon)
        args[ICON] = icon;
    else
        args[ICON - 1] = NULL;

    (void)runCmd("notify-send", args);
}

/****************.
|      Args      |
`****************/

static void help(int code) {
    fprintf(code ? stderr : stdout,
        "Usage: %s [OPTIONS...] BAT_PATH\n"
        "\n"
        "     BAT_PATH is the path to the battery capacity file.\n"
        "     E.g. `/sys/class/power_supply/BAT0/capacity'\n"
        "\n"
        "    -u, --urgent N            When should the urgent notification be sent.\n"
        "                              Set to -1 to turn off. Default: %d%%.\n"
        "    -w, --warning N           When should the warning notification be sent.\n"
        "                              Set to -1 to turn off. Default: %d%%.\n"
        "\n"
        "    -i, --interval N          How much time to wait between checking capacity.\n"
        "                              As decimal seconds. Default: %li.0s.\n"
        "\n"
        "        --no-icons            Do not put icons in the notification.\n"
        "        --urgent-icon S       Name or path of the warning notification icon.\n"
        "                              Default: %s.\n"
        "        --warning-icon S      Name or path of the warning notification icon.\n"
        "                              Default: %s.\n"
        "\n"
        "    -v, --version             Print progrm version and exit.\n"
        "    -h, --help                Print this message and exit.\n"
        "\n",
        progname,
        urgent_level,
        warning_level,
        interval.tv_sec,
        urgent_icon,
        warning_icon);
    exit(code);
}

static void version(void) {
    printf("%s v" BATNOTIFY_VERSION "\n", progname);
    exit(0);
}

static void readArgs(int argc, char **argv) {
    progname = basename(argv[0]);
    if (argc <= 1) help(1);
    for (int i = 1; i < argc; i++) {
        if (eq(argv[i], "-u") || eq(argv[i], "--urgent")) {
            if (++i >= argc) DIE("Expected number after %s\n", argv[i - 1]);
            urgent_level = getInt(argv[i], "Invalid integer");
        } else if (eq(argv[i], "-w") || eq(argv[i], "--warning")) {
            if (++i >= argc) DIE("Expected number after %s\n", argv[i - 1]);
            warning_level = getInt(argv[i], "Invalid integer");
        } else if (eq(argv[i], "-i") || eq(argv[i], "--interval")) {
            if (++i >= argc) DIE("Expected number after %s\n", argv[i - 1]);
            double t = getFloat(argv[i], "Invalid decimal");
            interval.tv_sec = t;
            interval.tv_nsec = 1e9 * (t - (double)interval.tv_sec);
        } else if (eq(argv[i], "--no-icons")) {
            urgent_icon = NULL;
            warning_icon = NULL;
        } else if (eq(argv[i], "--urgent-icon")) {
            if (++i >= argc) DIE("Expected path or name after %s\n", argv[i - 1]);
            urgent_icon = argv[i];
        } else if (eq(argv[i], "--warning-icon")) {
            if (++i >= argc) DIE("Expected path or name after %s\n", argv[i - 1]);
            warning_icon = argv[i];
        } else if (eq(argv[i], "-v") || eq(argv[i], "--version"))
            version();
        else if (eq(argv[i], "-h") || eq(argv[i], "--help"))
            help(0);
        else {
            if (BAT_PATH) DIE("Only one battery path can be specified\n");
            BAT_PATH = argv[i];
        }
    }
    if (!BAT_PATH) DIE("Specify path to battery capacity. Use -h for help.\n");
    snprintf(urgent_body,
        sizeof(urgent_body),
        "Battery level has dropped below %d%%. Please plug in the device or safely shut it down.",
        urgent_level);

    snprintf(warning_body,
        sizeof(warning_body),
        "Battery level has dropped below %d%%. Please plug in the device.",
        warning_level);
}
