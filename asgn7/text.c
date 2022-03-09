#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

#include "ht.h"
#include "bv.h"
#include "bf.h"
#include "parser.h"
#include "text.h"


#define WORD "[_a-zA-Z]+(['-][_a-zA-Z]+)*"

uint32_t noiselimit = 100;

struct Text{
	HashTable *ht;
	BloomFilter *bf;
	uint32_t word_count;
};

Text *text_create(FILE *infile, Text *noise){
	//Hashtable is an array of nodes
	Text *text = (Text *)malloc(sizeof(Text));
	if(text == NULL){
		return NULL;
	}
	text->ht = ht_create((0x1 << 19));	//1 x 2^19
	text->bf = bf_create((0x1 << 21));	//1 x 2^21
	text->word_count = 0;
	regex_t re;
	
	regcomp(&re, WORD, REG_EXTENDED);

	char *word = NULL;
	if(noise == NULL){	//if creating noise text
		while((word = next_word(infile, &re)) != NULL){
			int count = 0;
			while(word[count]){
				word[count] = tolower(word[count]);
				count += 1;
			}
			printf("Noise word = %s\n", word);
			ht_insert(text->ht, word);
			bf_insert(text->bf, word);
			text->word_count += 1;
			if(text->word_count == noiselimit){
				break;
			}
		}
		regfree(&re);
		return text;
	}
	else{	//if noise text is given
		while((word = next_word(infile, &re)) != NULL){
			int count = 0;
			while(word[count]){
				word[count] = tolower(word[count]);
				count += 1;
			}
			printf("text word = %s\n", word);
			//filter noise words out of text and add words to text, then return text
			if(text_contains(noise, word)){	//if word is inside the noise text, skip over that word
				continue;
			}
			else{	//if word is not inside noise, add it to the text
				ht_insert(text->ht, word);
				bf_insert(text->bf, word);
				text->word_count += 1;			
			}
			
		}
		regfree(&re);
		return text;
	}
}

void text_delete(Text **text){
	ht_delete(&(*text)->ht);
	bf_delete(&(*text)->bf);
	free(*text);
	*text = NULL;
}
/*
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
*/
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
	FILE *infile = fopen("test.txt", "r");
	FILE *infile1 = fopen("test1.txt", "r");
	Text *noisetext = text_create(infile, NULL);
	Text *text = text_create(infile1, noisetext);
	text_print(text);
	text_delete(&text);
	text_delete(&noisetext);
	fclose(infile);
	fclose(infile1);
	return 0;
}
