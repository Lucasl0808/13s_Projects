#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdbool.h>

#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "metric.h"
#include "node.h"
#include "pq.h"
#include "text.h"

int main(int argc, char **argv){
	int opt = 0;
	bool db_detect = false;
	bool noise_detect = false;
	FILE *database;
	FILE *noise;
	int matches = 5;
	//noiselimit = 100 defined in text.c
	Metric metric = EUCLIDEAN;
	while((opt = (getopt(argc, argv, "d:n:k:l:emch"))) != -1){
		switch(opt){
		case 'd':
			database = fopen(optarg, "r");
			if(database == NULL){
				fprintf(stderr, "error opening database file");
				return 1;
			}
			db_detect = true;
			break;
		case 'n':
			noise = fopen(optarg, "r");
			if(noise == NULL){
				fprintf(stderr, "error opening noise file");
				return 1;
			}
			noise_detect = true;
			break;
		case 'k':
			matches = strtol(optarg, NULL, 10);
			break;
		case 'l':
			noiselimit = strtol(optarg, NULL, 10);
			break;
		case 'e':
			metric = EUCLIDEAN;
			break;
		case 'm':
			metric = MANHATTAN;
			break;
		case 'c':
			metric = COSINE;
			break;
		case 'h':
			fprintf(stderr, "Program Details:\n");
			fprintf(stderr, "generates k most likely authors who wrote a given text\n");
			fprintf(stderr, "USAGE\n");
			fprintf(stderr, "-d	Specify database of authors and texts(default = lib.db)\n");
			fprintf(stderr, "-n	Specify file that contains noise words to filter out text (default = noise.txt)\n");
			fprintf(stderr, "-k	Specify number of authors to match to the given text (default = 5)\n");
			fprintf(stderr, "-l	Specify the number of noise words to filter out (default = 100)\n");
			fprintf(stderr, "-e	Set distance metric used to Euclidean distance(This is default)\n");
			fprintf(stderr, "-m	Set distance metric used to Manhattan distance\n");
			fprintf(stderr, "-c	Set distance metric used to Cosine distance\n");
			fprintf(stderr, "-h	Display Program details and usage help\n");
			return 1;
		default:
			fprintf(stderr, "enter command -h to display help and type a valid argument\n");
			return 0;
		}
	}
	//begin main file
}
