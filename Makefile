# GNU Makefile for bef.

PROGS=bin/bef bin/befprof bin/bef2c
CC?=gcc

WARNS=	-W -Wall -Wstrict-prototypes -Wmissing-prototypes \
	-Wpointer-arith	-Wno-uninitialized -Wreturn-type -Wcast-qual \
	-Wwrite-strings -Wswitch -Wcast-align -Wchar-subscripts \
	-Winline -Wnested-externs -Wredundant-decls

ifdef ANSI
  CFLAGS+= -ansi -pedantic
else
  CFLAGS+= -std=c99 -D_POSIX_C_SOURCE=200112L
endif

CFLAGS+= ${WARNS} ${EXTRA_CFLAGS}

ifdef DEBUG
  CFLAGS+= -g
endif

all: $(PROGS)

bin:
	mkdir -p bin

bin/bef: bin src/bef.c
	$(CC) $(CFLAGS) src/bef.c -o bin/bef

bin/befprof: bin src/befprof.c
	$(CC) $(CFLAGS) src/befprof.c -o bin/befprof

bin/bef2c: bin src/bef2c.c
	$(CC) $(CFLAGS) src/bef2c.c -o bin/bef2c

clean:
	rm -f src/*.o

distclean:
	rm -rf bin/*
