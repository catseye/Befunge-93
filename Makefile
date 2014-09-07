# GNU Makefile for bef.  No -Wshadow because horrible use of macros.

PROGS=bin/bef bin/befprof bin/bef2c
CC?=gcc
O?=.o
RM_F?=rm -f

WARNS=	-W -Wall -Wstrict-prototypes -Wmissing-prototypes \
	-Wpointer-arith	-Wno-uninitialized -Wreturn-type -Wcast-qual \
	-Wwrite-strings -Wswitch          -Wcast-align -Wchar-subscripts \
	-Winline -Wnested-externs -Wredundant-decls

ifdef DJGPP
else
  ifdef ANSI
    CFLAGS+= -ansi -pedantic
  else
    CFLAGS+= -std=c99 -D_POSIX_C_SOURCE=200809L
  endif
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
	$(RM_F) *$(O) src/*$(O)

distclean:
	$(RM_F) $(PROGS)
