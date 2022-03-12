CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror

.PHONY: all

all: integrate

integrate: integrate.o mathlib.o functions.o 
	$(CC) -o integrate integrate.o mathlib.o functions.o -lm

integrate.o: integrate.c 
	$(CC) $(CFLAGS) -c integrate.c 

mathlib.o: mathlib.c 
	$(CC) $(CFLAGS) -c mathlib.c

functions.o: functions.c
	$(CC) $(CFLAGS) -c functions.c

clean: 
	rm -f integrate integrate.o mathlib.o functions.o

format: 
	clang-format -i -style=file *.[ch]
