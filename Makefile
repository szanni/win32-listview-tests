.PHONY = all clean

CC = x86_64-w64-mingw32-gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDFLAGS = -lcomctl32

all: simple.exe

clean:
	rm -f simple.exe

%.exe: %.c
	${CC} ${CFLAGS} $^ -o $@ ${LDFLAGS}
