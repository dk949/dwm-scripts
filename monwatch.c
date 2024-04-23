/*
 * Based on https://gist.github.com/mafrasi2/4ee01e0ba4dad20cf7a80ae463f32fca
 */

#include "common.h"

#ifndef MONWATCH_VERSION
#    define MONWATCH_VERSION "UNKNOWN"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcb/randr.h>
#include <xcb/xcb.h>

typedef struct Args {
    enum { RUN_SHELL, RUN_CMD } type;

    union {
        char *shell;
        char **cmd;
    };

} Args;

xcb_connection_t *init(char const *dpy_name);
void deinit(xcb_connection_t *conn, Args *);
void mainLoop(xcb_connection_t *, Args *);
bool waitForScreenChange(xcb_connection_t *);

Args parseArgs(char **);

int main(int argc, char **argv) {
    (void)argc;

    setNice(19);

    Args args = parseArgs(argv);
    xcb_connection_t *conn = init(NULL);
    mainLoop(conn, &args);
    deinit(conn, &args);
}

xcb_connection_t *init(char const *dpy_name) {
    xcb_connection_t *conn = xcb_connect(dpy_name, NULL);
    switch (xcb_connection_has_error(conn)) {
        case XCB_CONN_ERROR: DIE("System error when connecting\n");
        case XCB_CONN_CLOSED_EXT_NOTSUPPORTED: DIE("Extension not supported\n");
        case XCB_CONN_CLOSED_MEM_INSUFFICIENT: DIE("Out of memory\n");
        case XCB_CONN_CLOSED_REQ_LEN_EXCEED: DIE("Request too long\n");
        case XCB_CONN_CLOSED_PARSE_ERR: DIE("Could not parse display string %s\n", dpy_name);
        case XCB_CONN_CLOSED_INVALID_SCREEN: DIE("No screen on the display\n");
    }
    xcb_setup_t const *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_window_t root = iter.data->root;
    xcb_randr_notify_mask_t mask = XCB_RANDR_NOTIFY_MASK_SCREEN_CHANGE;

    // This request does never generate any errors.
    xcb_randr_select_input(conn, root, mask);

    xcb_flush(conn);
    return conn;
}

bool waitForScreenChange(xcb_connection_t *conn) {
    static xcb_timestamp_t ts = 0;
    bool do_return = false;
    while (true) {
        xcb_generic_event_t *evt = xcb_wait_for_event(conn);
        if (!evt)
            return false;
        else {
            uint8_t type = evt->response_type;
            if (type & XCB_RANDR_NOTIFY_MASK_SCREEN_CHANGE) {
                xcb_timestamp_t ets = ((xcb_randr_screen_change_notify_event_t *)evt)->config_timestamp;
                if (ts != ets) {
                    ts = ets;
                    do_return = true;
                }
            }
            free(evt);
            if (do_return) return true;
        }
    }
}

void deinit(xcb_connection_t *conn, Args *args) {
    xcb_disconnect(conn);
    if (args->type == RUN_SHELL) {
        free(args->shell);
    }
}

void mainLoop(xcb_connection_t *conn, Args *args) {
    while (waitForScreenChange(conn)) {
        switch (args->type) {
            case RUN_SHELL: runShell(args->shell); break;
            case RUN_CMD: runCmd(*args->cmd, args->cmd); break;
        }
    }
}

Args parseArgs(char **argv) {
    Args args = {0};
    char const *prog_name = *(argv++);
    bool do_cat = false;
    for (char *arg = *argv; arg; arg = *(++argv)) {
        if (do_cat) {
            args.shell = catAlloc(args.shell, " ");
            args.shell = catAlloc(args.shell, arg);
        } else {
            if (eq(arg, "-h") || eq(arg, "--help")) {
                printf("Usage: %s\n"
                       "\n"
                       "    -c, --cmd           Interpret the following list of arguments as a command\n"
                       "    -s, --shell         Interpret the following list of arguments as a shell expression\n"
                       "\n"
                       "    -h, --help          Print this message and exit\n"
                       "    -v, --version       Print version number and exit\n"
                       "\n",
                    prog_name);
                exit(0);
            } else if (eq(arg, "-v") || eq(arg, "--version")) {
                printf("%s v" MONWATCH_VERSION "\n", prog_name);
                exit(0);
            } else if (eq(arg, "-s") || eq(arg, "--shell")) {
                if (!(++argv)) DIE("Expected a shell expression after %s\n", *(argv - 1));
                args.type = RUN_SHELL;
                args.shell = catAlloc(NULL, *argv);
                do_cat = true;
            } else if (eq(arg, "-c") || eq(arg, "--cmd")) {
                if (!(++argv)) DIE("Expected a command after %s\n", *(argv - 1));
                args.type = RUN_CMD;
                args.cmd = argv;
                break;
            }
        }
    }
    if (!args.cmd) DIE("Missing command to execute. See --help for help\n");

    return args;
}
