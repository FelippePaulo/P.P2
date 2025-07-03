CFLAGS =
ALL_CFLAGS = -O0 -g $(CFLAGS) -I. -Iinclude -ILibPPC/include -fopenmp

LDFLAGS =
ALL_LDFLAGS = $(LDFLAGS) LibPPC/lib/static/libppc.a -fopenmp

CC=gcc
LD=gcc

# passar como parametro do Makefile o nome do codigo fonte
HEADERS = LibPPC/include/libppc.h
LIBRARIES = LibPPC/lib/static/libppc.a
SRC = bubblesort.c
OBJ = $(SRC:.c=.o)

VPATH = src

.PHONY: all clean distclean

%.o: %.c $(HEADERS)
	$(CC) $(ALL_CFLAGS) -c $< -o $@

all: $(OBJ) $(LIBRARIES)
	gcc $< -o bubblesort $(ALL_LDFLAGS)

LibPPC/lib/static/libppc.a:
	make -C LibPPC static

clean:
	rm -f *.o src/*.o bubblesort

distclean: clean
	rm -f *.dat
	make -C LibPPC clean
