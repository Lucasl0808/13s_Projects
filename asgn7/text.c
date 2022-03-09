#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

#include "parser.h"
#include "text.h"


#define WORD "[_a-zA-Z]+(['-][_a-zA-Z]+)*"

struct Text{
	HashTable *ht;
	BloomFilter *bf;
	uint32_t word_count;
};

Text *text_create(FILE *infile, Text *noise){
	//Hashtable is an array of nodes
	Text *text = (Text *)malloc(sizeof(Text));
	if(test == NULL){
		return NULL;
	}
	text->ht = ht_create((0x1 << 19));	//1 x 2^19
	text->bf = bf_create((0x1 << 21));	//1 x 2^21
	text->word_count = 0;
	regex_t re;
	
	regcomp(&re, WORD, REG_EXTENDED);

	char *word = NULL;
	while((word = next_word(infile, &re)) != NULL){
		word = tolower(word);

		if(noise == NULL){
			//add all read words to the text and return it, text will be the noise
			ht_insert(text->ht, word);
			bf_insert(text->bf, word);
			text->word_count += 1;
		}
		else{
			//filter noise words out of text and add words to text, then return text
			if(ht_lookup(noise->ht, word) == NULL){
				//if word is not inside the noise...
				ht_insert(text->ht, word);
				bf_insert(text->bf, word);
				text->word_count += 1;
			}
		}
	}
}

void text_delete(Text **text){
	ht_delete(&(*text)->ht);
	bf_delete(&(*text)->bf);
	free(*text);
	*text = NULL;
}

double text_dist(Text *text1, Text *text2, Metric metric){
	//figure out how to access metric value
	//just access them using EUCLIDEAN, COSINE, or MANHATTAN
	if(metric == COSINE){

	}
	else if(metric == EUCLIDEAN){

	}
	else if(metric == MANHATTAN){

	}
	
}

bool text_contains(Text *text, char *word){
	if(bf_probe(text->bf, word)){
		if(ht_lookup(text->ht, word)){
			return true;
		}
		else{
			return false;	//false positive
		}
	}
	else{
		return false;
	}
}

void text_print(Text *text){
	ht_print(text->ht);
	bf_print(text->bf);
	printf("word count = %"PRIu32"\n", text->word_count);
}

int main(void){
	
}
