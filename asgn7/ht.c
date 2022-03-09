#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "node.h"
#include "ht.h"
#include "salts.h"
#include "speck.h"

struct HashTable{
	uint64_t salt[2];
	uint32_t size;
	Node **slots;
};

struct HashTableIterator{
	HashTable *table;
	uint32_t slot;
};

HashTable *ht_create(uint32_t size){
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
	ht->size = size;
	ht->salt[0] = SALT_HASHTABLE_LO;
	ht->salt[1] = SALT_HASHTABLE_HI;
	ht->slots = (Node **)malloc(sizeof(Node *) * size);
	for(uint32_t i = 0; i < ht->size; i +=1){
		ht->slots[i] = NULL;
	}
	return ht;
}

HashTableIterator *hti_create(HashTable *ht){
	HashTableIterator *hti = (HashTableIterator *)malloc(sizeof(HashTableIterator));
	hti->table = ht;
	hti->slot = 0;	
	return hti;
}

void hti_delete(HashTableIterator **hti){
	free(*hti);
}

void ht_delete(HashTable **ht){
	for(uint32_t i = 0; i < ht_size(*ht); i +=1){
		if((*ht)->slots[i] != NULL){
			node_delete(&(*ht)->slots[i]);
		}
	}
	//free all nodes in hash table slots
	free((*ht)->slots);
	free(*ht);
	*ht = NULL;

}

uint32_t ht_size(HashTable *ht){
	return ht->size;
}

Node *ht_lookup(HashTable *ht, char *word){
	int index = hash(ht->salt, word) % ht_size(ht);	//hash the word to get index and make sure that it is in the bounds of the hash table size
	uint32_t count = 0;
	while(count != ht_size(ht)){
		if(ht->slots[index] != NULL){
			if(strcmp(ht->slots[index]->word, word) == 0){	//if found word
				return ht->slots[index];
			}
		}
		index += 1;
		index = index % ht_size(ht);
		count += 1;
	}
	return NULL;
}

Node *ht_insert(HashTable *ht, char *word){
	int index = hash(ht->salt, word) % ht_size(ht);
	uint32_t count = 0;
	while(count != ht_size(ht)){
		if(ht->slots[index] == NULL){
			ht->slots[index] = node_create(word);
			ht->slots[index]->count = 1;
			return ht->slots[index];	//return pointer to the node at inserted index
		}
		else if(ht->slots[index] != NULL){
			if(strcmp(ht->slots[index]->word, word) == 0){	//if current node word matches word to insert
				ht->slots[index]->count += 1;
				return ht->slots[index];	//return pointer to the node at inserted index
			}
		}
		index += 1;
		index = index % ht_size(ht);
		count += 1;
	}
	return NULL;
}

void ht_print(HashTable *ht){
	printf("Size of Hashtable = %"PRIu32"\n", ht->size);
	printf("Lower Salt = %"PRIu64"\n", ht->salt[0]);
	printf("Upper Salt = %"PRIu64"\n", ht->salt[1]);
	printf("HashTable:\n");
	for(uint32_t i = 0; i < ht_size(ht); i += 1){
		if(ht->slots[i] == NULL){
			continue;
		}
		else{
			printf("Index %"PRIu32" = %s, count = %"PRIu32"\n", i, ht->slots[i]->word, ht->slots[i]->count);
		}
	}
}

Node *ht_iter(HashTableIterator *hti){
	while(hti->slot != hti->table->size){
		if(hti->table->slots[hti->slot] != NULL){
			uint32_t temp_slot = hti->slot;
			hti->slot += 1;
			return hti->table->slots[temp_slot];
			//return next node if available, and increment iterator slot
		}
		else{
			hti->slot += 1;
			//if node is NULL, dont return the NULL pointer, skip it by incrementing iterator slot
		}
	}
	return NULL;
	//if iterated through entire hash table, return NULL
}

/*
int main(void){
	//Node *a = NULL;
	HashTable *ht = ht_create(4);
	ht_insert(ht, "hello");
	ht_insert(ht, "world");
	HashTableIterator *hti = hti_create(ht);
	Node *n = NULL;
	while((n = ht_iter(hti)) != NULL){
		printf("%s\n", n->word);
	}
	hti_delete(&hti);
	//ht_insert(ht, "World");
	//a = ht_lookup(ht, "World");
	//node_print(a);
	//ht_print(ht);
	ht_delete(&ht);
	return 0;
}
*/
