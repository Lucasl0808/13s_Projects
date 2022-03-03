CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: encode decode

encode: encode.o node.o pq.o stack.o huffman.o code.o io.o
	$(CC) -o encode encode.o node.o pq.o stack.o huffman.o code.o io.o

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode: decode.o node.o pq.o stack.o huffman.o code.o io.o
	$(CC) -o decode decode.o node.o stack.o huffman.o code.o io.o

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[ch]

