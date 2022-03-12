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

struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

Text *text_create(FILE *infile, Text *noise) {
    //Hashtable is an array of nodes
    Text *text = (Text *) malloc(sizeof(Text));
    if (text == NULL) {
        return NULL;
    }
    text->ht = ht_create((0x1 << 19)); //1 x 2^19
    text->bf = bf_create((0x1 << 21)); //1 x 2^21
    text->word_count = 0;
    regex_t re;

    regcomp(&re, WORD, REG_EXTENDED);

    char *word = NULL;
    if (noise == NULL) { //if creating noise text
        while ((word = next_word(infile, &re)) != NULL) {
            int count = 0;
            while (word[count]) {
                word[count] = tolower(word[count]);
                count += 1;
            }
            //printf("Noise word = %s\n", word);
            ht_insert(text->ht, word);
            bf_insert(text->bf, word);
            text->word_count += 1;
            if (text->word_count == noiselimit) {
                break;
            }
        }
        regfree(&re);
        return text;
    } else { //if noise text is given
        while ((word = next_word(infile, &re)) != NULL) {
            int count = 0;
            while (word[count]) {
                word[count] = tolower(word[count]);
                count += 1;
            }
            //printf("text word = %s\n", word);
            //filter noise words out of text and add words to text, then return text
            if (text_contains(
                    noise, word)) { //if word is inside the noise text, skip over that word
                continue;
            } else { //if word is not inside noise, add it to the text
                ht_insert(text->ht, word);
                bf_insert(text->bf, word);
                text->word_count += 1;
            }
        }
        regfree(&re);
        return text;
    }
}

void text_delete(Text **text) {
    ht_delete(&(*text)->ht);
    bf_delete(&(*text)->bf);
    free(*text);
    *text = NULL;
}

double text_dist(Text *text1, Text *text2, Metric metric) {
    //just access them using EUCLIDEAN, COSINE, or MANHATTAN
    //in each, run a loop for each hash table, using a hash table iterator to get each word in the table
    //iterate over each hash table and check frequency of each word and compare
    HashTableIterator *hti = hti_create(text1->ht);
    HashTableIterator *hti2 = hti_create(text2->ht);
    double sum = 0;
    Node *n = NULL;

    if (metric == COSINE) {
        while ((n = ht_iter(hti)) != NULL) {
            double freq1 = text_frequency(text1, n->word);
            double freq2 = text_frequency(text2, n->word);
            sum += (freq1 * freq2);
        }
        while ((n = ht_iter(hti2)) != NULL) {
            if (text_contains(text1, n->word)) {
                continue;
            } else {
                double freq1 = text_frequency(text1, n->word);
                double freq2 = text_frequency(text2, n->word);
                sum += (freq1 * freq2);
            }
        }
        sum = 1 - sum;
        hti_delete(&hti);
        hti_delete(&hti2);
        return sum;
    } else if (metric == EUCLIDEAN) {
        while ((n = ht_iter(hti)) != NULL) {
            double freq1 = text_frequency(text1, n->word);
            double freq2 = text_frequency(text2, n->word);
            double temp = (freq1 - freq2);
            temp = (temp * temp);
            sum += temp;
        }
        while ((n = ht_iter(hti2)) != NULL) {
            if (text_contains(text1, n->word)) {
                continue;
            } else {
                double freq1 = text_frequency(text1, n->word);
                double freq2 = text_frequency(text2, n->word);
                double temp = (freq1 - freq2);
                temp = (temp * temp);
                sum += temp;
            }
        }
        sum = sqrt(sum);
        hti_delete(&hti);
        hti_delete(&hti2);
        return sum;
    } else if (metric == MANHATTAN) {
        while ((n = ht_iter(hti)) != NULL) { //iterate through text 1's hash table
            sum += fabs((text_frequency(text1, n->word)) - text_frequency(text2, n->word));
        }
        while ((n = ht_iter(hti2))
               != NULL) { //iterate through text2, if word appeared in text 1 ignore it
            if (text_contains(text1, n->word)) {
                continue;
            } else {
                sum += fabs((text_frequency(text1, n->word) - text_frequency(text2, n->word)));
            }
        }
        hti_delete(&hti);
        hti_delete(&hti2);
        return sum;
    }
    hti_delete(&hti);
    hti_delete(&hti2);
    return sum;
}

double text_frequency(Text *text, char *word) {
    if (text_contains(text, word) == false) { //if word is not inside text
        return 0;
    } else {
        Node *n = ht_lookup(text->ht, word);
        double freq = (double) n->count / (double) text->word_count;
        return freq;
    }
}
bool text_contains(Text *text, char *word) {
    if (bf_probe(text->bf, word)) {
        if (ht_lookup(text->ht, word)) {
            return true;
        } else {
            return false; //false positive
        }
    } else {
        return false;
    }
}

void text_print(Text *text) {
    ht_print(text->ht);
    bf_print(text->bf);
    printf("word count = %" PRIu32 "\n", text->word_count);
}
/*
int main(void){
	FILE *infile = fopen("test.txt", "r");
	FILE *infile1 = fopen("test1.txt", "r");
	FILE *infile2 = fopen("test2.txt", "r");
	Text *noisetext = text_create(infile, NULL);
	Text *text = text_create(infile1, noisetext);
	Text *text2 = text_create(infile2, noisetext);
	text_print(text);
	printf("text distance = %.15f\n", text_dist(text, text2, EUCLIDEAN));
	text_delete(&text);
	text_delete(&text2);
	text_delete(&noisetext);
	fclose(infile);
	fclose(infile1);
	return 0;
}
*/
