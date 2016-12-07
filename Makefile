CC = gcc
CFLAGS= -Wall -Werror -g

btree: huffman.c huffcode.c 
	$(CC) $(CFLAGS) -o huffman huffman.c huffcode.c

clean:
	rm -f huffman