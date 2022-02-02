CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror

.PHONY: all

all: life

life: life.o
	$(CC) -o life life.o universe.c -lncurses

life.o: life.c
	$(CC) $(CFLAGS) -c life.c -o life.o

clean:
	rm -f life life.o

format:
	clang-format -i -style=file *.[ch]
