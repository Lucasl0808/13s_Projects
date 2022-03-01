#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "header.h"
#include "defines.h"
#include "node.h"
#include "io.h"
#include "pq.h"
#include "stack.h"

int main(int argc, char **argv){
	int opt = 0;
	//open returns fd
	int infile = open(STDIN_FILENO, O_RDONLY);	//default infile = stdin
	int outfile = open(STDOUT_FILENO, O_RDWR | O_CREAT | O_TRUNC);	//default outfile = stdout
	bool verbose = false;
	while((opt = (getopt(argc, argv, "hi:o:v"))) != -1){
		switch(opt){
		case 'h':
			fprintf(stderr, "Synopsis\n");
			fprintf(stderr, "	Uses Huffman encoding and decoding to compress files and decode/encode messages\n");
			fprintf(stderr, "\nUSAGE\n");
			fprintf(stderr, "-h	Prints out program synopsis/usage details\n");
			fprintf(stderr, "-i infile	specify input file to use Huffman encoding on\n");
			fprintf(stderr, "-o outfile	specify output file to write compressed file from huffman encoding to\n");
			fprintf(stderr, "-v	prints file compression statistics to stderr. Prints uncompressed file size, compressed file size, and space saving\n");
			return 1;
		case 'i':
			infile = open(optarg, O_RDONLY);	//infile = specified file for reading
			if(infile == -1){
				fprintf(stderr, "error opening infile\n");
				return 0;
			}
			break;
		case 'o':
			outfile = open(optarg, O_RDWR | O_CREAT | O_TRUNC);	//outfile  = specified file for writing
			if(outfile == -1){
				fprintf(stderr, "error opening outfile\n");
				return 0;
			}
			break;
		case 'v':
			verbose = true;
			break;
		default:
			fprintf(stderr, "invalid input, try again with -h to see valid arguments\n");
			return 0;
		}
	}
	uint64_t hist[ALPHABET];	//create histogram of size 256(alphabet). Histogram = array of uint64_t's
	for(int i=0; i < ALPHABET; i += 1){
		hist[i] = 0;	//initialize all values of histogram to 0 before reading in infile
	}

	//read through infile to fill up hist with data. Index of histogram = symbol
	//maybe read into a buffer and read each index of that buffer
	uint8_t buffer[BLOCK];
	for(int i = 0; i < BLOCK; i += 1){
		buffer[i] = 0;	//initialize empty buffer
	}
	

	/*
	int read = read_bytes(infile, &buffer, BLOCK);	//read in a BLOCK of bytes from infile and fill up buffer with values read

	if(bytes_read == BLOCK){
		for(int i = 0; i < BLOCK; i += 1){
			hist[buffer[i]] += 1;
			buffer[i] = 0;
		}
	}
	*/
	
	//reads infile contents and creates histogram
	int read = 0;
	while((read = read_bytes(infile, buffer, BLOCK)) > 0){
		for(int i = 0; i < read; i += 1){
			hist[buffer[i]] += 1;
		}
	}

	hist[0] += 1;
	hist[255] += 1;	//increment end indices of hist

	Node *root = build_tree(hist);

	Code table[ALPHABET];

	build_codes(root, table);	//build code table using huffman tree root and table

	Header h;	//initialize header
	h.magic = MAGIC;
	
	struct stat file_stat;

	fstat(infile, &file_stat);

	h.permissions = file_stat.st_mode;
	fchmod(outfile, file_stat.st_mode);	//set permissions = to mode given by fstat

	uint16_t count = 0;
	for(int i = 0; i < ALPHABET; i += 1){	//get number of unique symbols in histogram
		if(hist[i] > 0){
			count += 1;
		}
	}
	count *= 3;
	count -= 1;	//calculations to get tree_size
	h.tree_size = count;

	h.file_size = file_stat.st_size;	//set file size = to size in bytes given by fstat

	//figure out how to write header to outfile. Maybe write each of the individual aspects of header to outfile separately
	write_bytes(outfile, &h, sizeof(h));	//writes header to outfile
	
	//dump tree using root node and outfile
	dump_tree(outfile, root);

	//START HERE AT 10.1.8 on ASSIGNMENT DOC

}
