# GNU Makefile for bef.  No -Wshadow because horrible use of macros.

PROGS=bin/bef$(EXE) bin/befprof$(EXE) bin/bef2c$(EXE)
CC?=gcc
O?=.o
EXE?=
RM_F?=rm -f

WARNS=	-W -Wall -Wstrict-prototypes -Wmissing-prototypes \
	-Wpointer-arith	-Wno-uninitialized -Wreturn-type -Wcast-qual \
	-Wwrite-strings -Wswitch          -Wcast-align -Wchar-subscripts \
	-Winline -Wnested-externs -Wredundant-decls

ifdef ANSI
  CFLAGS+= -ansi -pedantic -D_BSD_SOURCE
else
  CFLAGS+= -std=c99 -D_POSIX_C_SOURCE=200809L
endif

CFLAGS+= ${WARNS} ${EXTRA_CFLAGS}

ifdef DEBUG
  CFLAGS+= -g
endif

all: $(PROGS)

bin/bef: src/bef.c
	$(CC) $(CFLAGS) src/bef.c -o bin/bef

bin/befprof: src/befprof.c
	$(CC) $(CFLAGS) src/befprof.c -o bin/befprof

bin/bef2c: src/bef2c.c
	$(CC) $(CFLAGS) src/bef2c.c -o bin/bef2c

clean:
	$(RM_F) *.o src/*.o

distclean:
	$(RM_F) $(PROGS)
