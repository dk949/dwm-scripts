DESTDIR=$(HOME)/.local
PREFIX=


CC ?= gcc
CPPFLAGS=-DBATNOTIFY_VERSION=\"0.2.1\" -D_DEFAULT_SOURCE
CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused -Werror -Os -std=c99 $(CFLAGS)
