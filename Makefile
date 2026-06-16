include config.mk

SRC = utils.c config.c hhkb.c
OBJ = ${SRC:.c=.o}

all: options hhkb

options:
	@echo hhkb build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk config.h

config.h:
	cp -f config.def.h $@

hhkb: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	${RM} ${OBJ} hhkb-${VERSION}.tar.gz hhkb.1.gz

dist: clean
	mkdir -p hhkb-${VERSION}
	cp -R LICENSE config.mk Makefile README.md config.h hhkb.1 ${SRC} hhkb-${VERSION}
	tar -cf hhkb-${VERSION}.tar hhkb-${VERSION}
	gzip hhkb-${VERSION}.tar
	${RM} -r hhkb-${VERSION}

install: all
	sudo install -m755 hhkb ${PREFIX}/bin/
	mkdir -p ${HOME}/.config/systemd/user
	install -m644 hhkb.service ${HOME}/.config/systemd/user/
	systemctl --user disable --now hhkb 2>/dev/null || true
	systemctl --user daemon-reload
	systemctl --user enable --now hhkb

uninstall:
	${RM} ${PREFIX}/bin/hhkb ${MANPREFIX}/man1/hhkb.1.gz

.PHONY: all options clean dist install uninstall
