.PHONY = all clean

CC = x86_64-w64-mingw32-gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDFLAGS = -lcomctl32

BIN = simple.exe print-notify.exe

all: ${BIN}

clean:
	rm -f ${BIN}

%.exe: %.c
	${CC} ${CFLAGS} $^ -o $@ ${LDFLAGS}
