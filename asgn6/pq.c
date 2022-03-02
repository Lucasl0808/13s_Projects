#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "pq.h"
#include "node.h"

struct PriorityQueue{
	uint32_t capacity;
	uint32_t size;
	Node **array;
	
};

PriorityQueue *pq_create(uint32_t capacity){
	PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	q->capacity = capacity;
	q->size = 0;
	q->array = (Node **)malloc(sizeof(Node *)*capacity);
	return q;
}

void pq_delete(PriorityQueue **q){
	free((*q)->array);
	free(*q);
	*q = NULL;
}

bool pq_empty(PriorityQueue *q){
	if(q->size == 0){
		return true;
	}
	else{
		return false;
	}
}

bool pq_full(PriorityQueue *q){
	if(q->size == q->capacity){
		return true;
	}
	else{
		return false;
	}
}

uint32_t pq_size(PriorityQueue *q){
	return q->size;
}

bool enqueue(PriorityQueue *q, Node *n){
	//Find a way to implement sorting to queue. Run through q looking at each index's frequency
	//put Node n somewhere inside queue based on the frequency
	if(pq_full(q)){
		return false;
	}
	if(pq_empty(q)){
		q->array[0] = n;	//insert node into array if it is empty
		q->size += 1;
		return true;
	}
	/*
	uint32_t index = 0;
	while(n->frequency >= q->array[index]->frequency){	//while node freq is >= q frequency
		index += 1;	//move down array index to see what index to insert node into
	}
	q->size += 1;	//increase size to index rest of the array
	for(uint32_t i = pq_size(q); i > index; i -= 1){
		q->array[i] = q->array[i-1];	//move all elements past where Node n is inserted to the right by 1
	}
	q->array[index] = n;
	
	*/
	
	q->array[pq_size(q)] = n;	//insert Node n at the end of the array
	q->size += 1;
	
	//insertion sort to sort the nodes by frequency
	
	for(uint32_t i = 1; i < pq_size(q); i +=1){
		uint32_t j = i;
		Node *temp = q->array[i];	//create temp value too hold current Node being read
		while(j > 0 && temp->frequency < q->array[j-1]->frequency){
			q->array[j] = q->array[j-1];
			j -= 1;
		}
		q->array[j] = temp;
	}
	
	return true;
}

bool dequeue(PriorityQueue *q, Node **n){
	if(pq_empty(q)){
		return false;
	}
	*n = q->array[0];	//set n = highest prio array index
	//q->array[0] = NULL;	//item pulled out is removed from array
	for(uint32_t i = 1; i <= pq_size(q); i +=1){
		q->array[i-1] = q->array[i];	//shift all items in array left by one index to set the next highest priority to array[0]
		if(i == pq_size(q)){
			q->array[i] = NULL;	//sets the last array spot to NULL indicating a empty slot because it moved left one spot
		}
	}
	q->size -= 1;
	return true;
}

void pq_print(PriorityQueue *q){
	for(uint32_t i = 0; i < pq_size(q); i += 1){
		printf("Node Symbol = %c", q->array[i]->symbol);
		printf("Node freq = %"PRIu64"\n", q->array[i]->frequency);

	}
}
/*
int main(void){
	Node *z = node_create('^', 100);
	Node *a = node_create('!', 12);
	Node *b = node_create('@', 10);
	Node *c = node_create('#', 8);
	Node *l = node_create('?', 19);
	//Node *g = NULL;
	PriorityQueue *q = pq_create(5);
	if(pq_empty(q)){
		printf("empty\n");
	}
	if(enqueue(q, z) == false){
		return 1;
	}
	if(enqueue(q, a) == false){
		return 1;
	}
	if(enqueue(q, b) == false){
		return 1;
	}
	if(enqueue(q, c) == false){
		return 1;
	}
	//if(dequeue(q, &g) == false){
	//	return 1;
	//}
	enqueue(q, l);
	z = node_create('g', 2);
	enqueue(q, z);
	if(pq_full(q)){
		printf("full queue\n");
	}

	printf("%" PRIu32"\n", q->size);

	node_print(z);
//	free(g);
	for(uint32_t i = 0; i <= pq_size(q); i +=1){
		free(q->array[i]);
	}
	pq_print(q);
	pq_delete(&q);
	return 0;
}
*/
