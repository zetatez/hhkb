VERSION=1.0

## Customize below to fit your system
# protocol: X
PROTOCOL=X

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# x11
X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# freetype
FREETYPELIBS = -lfontconfig -lXft
FREETYPEINC = /usr/include/freetype2

# Xinerama
XINERAMALIBS  = -lXinerama
XINERAMAFLAGS = -DXINERAMA

# includes and libs
INCS = -I${X11INC}\
		-I${FREETYPEINC}

LIBS = -L${X11LIB}\
		${FREETYPELIBS}\
		${XINERAMALIBS}\
		-lX11\
		-lXi\
		-lXfixes\
		-lXext\
		-lXinerama\
		-lXi\
		-lXtst\
		-lX11\
		-lXft\
		-lXrandr

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=200809L ${XINERAMAFLAGS}
CFLAGS = -g -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
