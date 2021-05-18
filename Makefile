include config.mk
all:

clean:

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin/
	install highestUsageProcess ${DESTDIR}${PREFIX}/bin/highestUsageProcess
	install layout-check ${DESTDIR}${PREFIX}/bin/layout-check
	install makebg ${DESTDIR}${PREFIX}/bin/makebg
	install notify-lock ${DESTDIR}${PREFIX}/bin/notify-lock
	install picom-end ${DESTDIR}${PREFIX}/bin/picom-end
	install picom-start ${DESTDIR}${PREFIX}/bin/picom-start
	install turnoff ${DESTDIR}${PREFIX}/bin/turnoff
	install update-variables ${DESTDIR}${PREFIX}/bin/update-variables

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/highestUsageProcess
	rm -f -check ${DESTDIR}${PREFIX}/bin/layout-check
	rm -f ${DESTDIR}${PREFIX}/bin/makebg
	rm -f -lock ${DESTDIR}${PREFIX}/bin/notify-lock
	rm -f -end ${DESTDIR}${PREFIX}/bin/picom-end
	rm -f -start ${DESTDIR}${PREFIX}/bin/picom-start
	rm -f ${DESTDIR}${PREFIX}/bin/turnoff
	rm -f -variables ${DESTDIR}${PREFIX}/bin/update-variables

.PHONY: all clean install uninstall
