# GNU Makefile for bef.

PROGS=bef befprof bef2c
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

bef: bef.c
	$(CC) $(CFLAGS) bef.c -o bef

befprof: befprof.c
	$(CC) $(CFLAGS) befprof.c -o befprof

bef2c: bef2c.c
	$(CC) $(CFLAGS) bef2c.c -o bef2c

clean:
	rm -f *.o *.core *.exe $(PROGS)
