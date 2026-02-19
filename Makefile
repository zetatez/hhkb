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

${OBJ}: config.mk

config.h:
	cp -f config.def.h $@

hhkb: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${OBJ} hhkb-${VERSION}.tar.gz hhkb.1.gz

dist: clean
	mkdir -p hhkb-${VERSION}
	cp -R LICENSE config.mk Makefile README.md config.h hhkb.1 ${SRC} hhkb-${VERSION}
	tar -cf hhkb-${VERSION}.tar hhkb-${VERSION}
	gzip hhkb-${VERSION}.tar
	rm -rf hhkb-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m755 hhkb ${DESTDIR}${PREFIX}/bin/
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	cat hhkb.1 | gzip > hhkb.1.gz
	install -m644 hhkb.1.gz ${DESTDIR}${MANPREFIX}/man1/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/hhkb\
    		${DESTDIR}${MANPREFIX}/man1/hhkb.1.gz

.PHONY: all options clean
