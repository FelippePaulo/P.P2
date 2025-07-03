CFLAGS = 
ALL_CFLAGS = -O0 -g $(CFLAGS)

LDFLAGS = -static ../lib/static/libppc.a
ALL_LDFLAGS = $(LDFLAGS)

CC=gcc
LD=gcc

# passar como parametro do Makefile o nome do codigo fonte
SRC = matrixmult.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

%.o: %.c
	$(CC) -I. -I../include $(ALL_CFLAGS) -c $< -o $@

all: $(OBJ)
	$(LD) $? -o matrixmult $(LDFLAGS) 

clean:
	rm -f *.o matrixmult 


