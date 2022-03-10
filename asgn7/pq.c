#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "pq.h"

typedef struct PQindex PQindex;

struct PQindex{
	char *author;
	double dist;
};

struct PriorityQueue{
	uint32_t capacity;
	uint32_t size;
	PQindex *array;
};

PQindex index_create(char *author, double dist){
	PQindex PQindex;
	//PQindex *PQindex = (PQindex *)malloc(sizeof(PQindex));
	PQindex.author = author;
	PQindex.dist = dist;
	return PQindex;
}

void index_delete(PQindex **PQindex){
	free(*PQindex);
	*PQindex = NULL;
}

PriorityQueue *pq_create(uint32_t capacity){
	PriorityQueue *q = (PriorityQueue *)malloc(sizeof(PriorityQueue));
	q->capacity = capacity;
	q->size = 0;
	q->array = (PQindex *)malloc(sizeof(PQindex) * capacity);
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

bool enqueue(PriorityQueue *q, char *author, double dist){
	if(pq_full(q)){
		return false;
	}
	if(pq_empty(q)){
		q->array[0] = index_create(author, dist);
		q->size += 1;
		return true;
	}

	q->array[pq_size(q)] = index_create(author, dist);
	q->size += 1;

	for(uint32_t i = 1; i < pq_size(q); i+= 1){
		uint32_t j = i;
		PQindex temp = q->array[i];
		while(j > 0 && temp.dist < q->array[j - 1].dist){
			q->array[j] = q->array[j-1];
			j -= 1;
		}
		q->array[j] = temp;
	}
	return true;
}

uint32_t pq_size(PriorityQueue *q){
	return q->size;
}

void index_print(PQindex *PQindex){
	printf("author = %s\n", PQindex->author);
	printf("distance = %.15f\n", PQindex->dist);
}

void pq_print(PriorityQueue *q){
	for(uint32_t i = 0; i < pq_size(q); i += 1){
		printf("index author = %s, dist = %.15f\n", q->array[i].author, q->array[i].dist);
	}
}
int main(void){
	PriorityQueue *q = pq_create(4);
	PQindex one = index_create("Kanye", 0.0023);
	one.dist = 0.0245;
	index_print(&one);
	enqueue(q, one.author, one.dist);
	pq_print(q);
	return 1;
}
