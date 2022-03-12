CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: identify

identify: identify.o node.o bf.o bv.o ht.o parser.o pq.o speck.o text.o
	$(CC) -o identify identify.o node.o bf.o bv.o ht.o parser.o pq.o speck.o text.o -lm -Ofast

identify.o: identify.c
	$(CC) $(CFLAGS) -c identify.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

text.o: text.c
	$(CC) $(CFLAGS) -c text.c

clean:
	rm -f identify *.o

format:
	clang-format -i -style=file *.[ch]
