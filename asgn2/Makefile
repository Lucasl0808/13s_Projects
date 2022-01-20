CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror

.PHONY: all

all: integrate

integrate: integrate.o mathlib.o 
	$(CC) -o integrate integrate.o mathlib.o functions.c -lm

integrate.o: integrate.c 
	$(CC) $(CFLAGS) -c integrate.c -o integrate.o

mathlib.o: mathlib.c 
	$(CC) $(CFLAGS) -c mathlib.c -o mathlib.o 

clean: 
	rm - f integrate integrate.o

format: 
	clang-format -i -style=file integrate.c mathlib.c mathlib.h
