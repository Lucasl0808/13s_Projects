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
	q->array[0] = NULL;
	for(uint32_t i = 1; i <= pq_size(q); i +=1){
		q->array[i-1] = q->array[i];
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

int main(void){
	Node *z = node_create('^', 1);
	Node *a = node_create('!', 12);
	Node *b = node_create('@', 10);
	Node *c = node_create('#', 8);
	Node *g = NULL;
	PriorityQueue *q = pq_create(100);
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
	if(dequeue(q, &g) == false){
		return 1;
	}
	node_print(g);
	free(g);
	//free(z);
	free(a);
	free(b);
	free(c);
	pq_print(q);
	pq_delete(&q);
	return 0;
}
