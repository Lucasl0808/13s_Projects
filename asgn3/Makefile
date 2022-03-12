CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror

.PHONY: all

all: sorting

sorting: sorting.o 
	$(CC) -o sorting sorting.o quick.c insert.c heap.c batcher.c stats.c -lm

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c -o sorting.o

clean:
	rm -f sorting sorting.o

format:
	clang-format -i -style=file *.[ch]
