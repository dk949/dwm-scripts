include config.mk
all: batnotify monwatch

clean:
	rm -f batnotify
	rm -f monwatch

batnotify: batnotify.c common.h config.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@

monwatch: monwatch.c common.h config.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	$(foreach file, $(wildcard *.sh), install $(file) $(DESTDIR)$(PREFIX)/bin/$(basename $(file));)
	install batnotify $(DESTDIR)$(PREFIX)/bin/batnotify
	install monwatch $(DESTDIR)$(PREFIX)/bin/monwatch

uninstall:
	$(foreach file, $(wildcard *.sh), rm -f $(DESTDIR)$(PREFIX)/bin/$(basename $(file));)
	rm -f $(DESTDIR)$(PREFIX)/bin/batnotify

.PHONY: all clean install uninstall
