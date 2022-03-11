#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

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
	int metric_i = 1;
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
			metric_i = 2;
			break;
		case 'c':
			metric = COSINE;
			metric_i = 3;
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
	if(db_detect == false){
		database = fopen("lib.db", "r");
	}
	if(noise_detect == false){
		noise = fopen("noise.txt", "r");
	}
	//create noise text
	Text *noisetext = text_create(noise, NULL);
	
	//create text from infile passed into stdin
	//FILE *infile = stdin;
	Text *text1 = text_create(stdin, noisetext);
	
	//scan first line from database
	uint32_t n;
	fscanf(database, "%"SCNu32"\n", &n);
	
	//create priority queue that holds n elements
	PriorityQueue *q = pq_create(n);

	//use fgets() in a loop to read an author, and use fgets() again to get the author's text and remove the trailing newline
	uint32_t pairs = 0;
	//char *temp = NULL;
	while(pairs != n){
		char *temp = NULL;
		char author[2048];
		fgets(author, sizeof(author), database);
		//replace second to last element in the string with a null character(second to last element is newline char)
		author[strlen(author) - 1] = '\0';
		temp = strndup(author, 2048);

		char authortext[2048];
		fgets(authortext, sizeof(authortext), database);
		authortext[strlen(authortext) -1] = '\0';
		FILE *text2_file = fopen(authortext, "r");
		if(text2_file == NULL){
			pairs += 1;
			continue;
		}
		Text *text2 = text_create(text2_file, noisetext);
		double dist = text_dist(text1, text2, metric);
		enqueue(q, temp, dist);
		fclose(text2_file);
		text_delete(&text2);
		pairs += 1;
		//free(temp);
	}
	
	char *metric_str;
	if(metric_i == 1){
		metric_str = "Euclidean distance";
	}
	else if(metric_i == 2){
		metric_str = "Manhattan distance";
	}
	else{
		metric_str = "Cosine distance";
	}
	printf("Top %d, metric: %s, noise limit: %"PRIu32"\n", matches, metric_str, noiselimit);
	for(int i = 0; i < matches; i +=1){
		char *author;
		double dist = 0;
		dequeue(q, &author, &dist);
		printf("%d) %s [%.15f]\n", i+1, author, dist);
		free(author);
	}
	text_delete(&text1);
	text_delete(&noisetext);
	fclose(database);
	fclose(noise);
	pq_delete(&q);
	return 1;
	
}
