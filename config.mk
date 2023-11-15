DESTDIR=$(HOME)/.local
PREFIX=


CC ?= gcc
CPPFLAGS=-DBATNOTIFY_VERSION=\"0.2.1\" -DMONWATCH_VERSION=\"0.1.0\" -D_DEFAULT_SOURCE -DMAXMONS=8
CFLAGS := -Wall -Wextra -Wpedantic -Wno-unused -Werror -Os -std=c99 $(CFLAGS)
