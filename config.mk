DESTDIR=$(HOME)/.local
PREFIX=


CC ?= gcc
CPPFLAGS=-DBATNOTIFY_VERSION=\"0.1.1\" -D_DEFAULT_SOURCE
CFLAGS := -Wall -Wextra -Wpedantic -Werror -Os -std=c99 $(CFLAGS)
