CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: encrypt decrypt keygen

encrypt: encrypt.o numtheory.o rsa.o
	$(CC) -o encrypt encrypt.o numtheory.o rsa.o $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: decrypt.o numtheory.o rsa.o
	$(CC) -o decrypt decrypt.o numtheory.o rsa.o $(LFLAGS)

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c

keygen: keygen.o numtheory.o rsa.o randstate.o
	$(CC) -o keygen keygen.o numtheory.o rsa.o randstate.o $(LFLAGS)

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

clean:
	rm -f keygen decrypt encrypt *.o

format:
	clang-format -i -style=file *.[ch]
