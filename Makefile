include config.mk
all: batnotify

clean:
	rm -f batnotify

batnotify: batnotify.c config.mk
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	$(foreach file, $(wildcard *.sh), install $(file) $(DESTDIR)$(PREFIX)/bin/$(basename $(file));)
	install batnotify $(DESTDIR)$(PREFIX)/bin/batnotify

uninstall:
	$(foreach file, $(wildcard *.sh), rm -f $(DESTDIR)$(PREFIX)/bin/$(basename $(file));)
	rm -f $(DESTDIR)$(PREFIX)/bin/batnotify

.PHONY: all clean install uninstall
