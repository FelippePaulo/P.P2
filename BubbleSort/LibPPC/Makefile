CFLAGS = 
ALL_CFLAGS = -O0 -g $(CFLAGS)

LDFLAGS = 
ALL_LDFLAGS = $(LDFLAGS)

CC=gcc
LD=gcc

# passar como parametro do Makefile o nome do codigo fonte
HEADERS = include/libppc.h
SRC = libpcc.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean clean-obj static shared

VPATH = src

%.o: %.c $(HEADERS)
	$(CC) -I. -Iinclude $(ALL_CFLAGS) -c $< -o $@

all: 
	make clean static 
	make clean shared
	
static:
	mkdir -p lib/static
	make CFLAGS="$(CFLAGS)" $(OBJ)
	$(AR) cru lib/static/libppc.a $(OBJ)

shared:
	mkdir -p lib/shared
	make CFLAGS="$(CFLAGS) -fPIC" $(OBJ)
	$(LD) -shared $(ALL_LDFLAGS) $(OBJ) -o lib/shared/libppc.so

clean-obj:
	rm -rf *.o

clean-shared:
	rm -rf lib/shared/*

clean-static:
	rm -rf lib/static/*

clean: clean-obj clean-static clean-shared

install:
	echo "ok"
	


