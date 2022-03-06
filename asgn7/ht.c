#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "node.h"
#include "ht.h"
#include "hti.h"
#include "salts.h"
#include "speck.h"

struct HashTable{
	uint64_t salt[2];
	uint32_t size;
	Node **slots;
};

HashTable *ht_create(uint32_t size){
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	ht->size = size;
	ht->salt[0] = SALT_HASHTABLE_LO;
	ht->salt[1] = SALT_HASHTABLE_HI;
	ht->slots = (Node **)malloc(sizeof(Node *) * size);
	return ht;
}

void ht_delete(HashTable **ht){
	for(uint32_t i = 0; i < ht_size(*ht); i +=1){
		if((*ht)->slots[i] != NULL){
			free((*ht)->slots[i]);	
		}
	}
	//free all nodes in hash table slots
	free(*ht);
	*ht = NULL;

}

uint32_t ht_size(HashTable *ht){
	return ht->size;
}

Node *ht_lookup(HashTable *ht, char *word){
	int index = hash(ht->salt, word) % ht_size(ht);	//hash the word to get index and make sure that it is in the bounds of the hash table size
	int count = 0;
	while(count != ht_size(ht)){
		if(ht->slots[index]->word == word){
			return ht->slots[index]->word;
		}
		index += 1;
		index = index % ht_size(ht);
		count += 1;
	}
	return NULL;
}

Node *ht_insert(HashTable *ht, char *word){
	int index = hash(ht->salt, word) % ht_size(ht);
	int count = 0;
	while(count != ht_size(ht)){
		if(ht->slots[index] == NULL){
			ht->slots[index] = node_create(word, 1);

		}
	}
}

void ht_print(HashTable *ht){
	printf("Size of Hashtable = %"PRIu32"\n", ht->size);
	printf("Lower Salt = %"PRIu64"\n", ht->salt[0]);
	printf("Upper Salt = %"PRIu64"\n", ht->salt[1]);
	printf("HashTable:\n");
	for(uint32_t i = 0; i < ht->size; i += 1){
		printf("Index %"PRIu32" = %s, count = %"PRIu32"\n", i, ht->slots[i]->word, ht->slots[i]->count);
	}
}

int main(void){
	//Node *a = node_create("Hello");
	//Node *b = node_create("World");
	HashTable *ht = ht_create(2);
	ht_print(ht);
	return 0;
}
