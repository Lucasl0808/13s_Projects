#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

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

	hist[0] += 1;
	hist[255] += 1;	//increment end indices of hist


}
