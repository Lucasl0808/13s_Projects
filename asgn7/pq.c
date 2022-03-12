#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "pq.h"

typedef struct PQindex PQindex;

struct PQindex { //local struct to hold author and corresponding distance
    char *author;
    double dist;
};

struct PriorityQueue {
    uint32_t capacity;
    uint32_t size;
    PQindex *array;
};

PQindex index_create(char *author, double dist) { //constructor for local struct
    PQindex PQindex;
    PQindex.author = author;
    PQindex.dist = dist;
    return PQindex;
}
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    q->capacity = capacity;
    q->size = 0;
    q->array = (PQindex *) malloc(sizeof(PQindex) * capacity);
    return q;
}

void pq_delete(PriorityQueue **q) {
    while (pq_size(*q) > 0) {
        char *temp = NULL;
        double temp1 = 0;
        dequeue(*q, &temp, &temp1);
        free(temp);
    }
    free((*q)->array);
    free(*q);
    *q = NULL;
}

bool pq_empty(PriorityQueue *q) {
    if (q->size == 0) {
        return true;
    } else {
        return false;
    }
}

bool pq_full(PriorityQueue *q) {
    if (q->size == q->capacity) {
        return true;
    } else {
        return false;
    }
}

bool enqueue(PriorityQueue *q, char *author, double dist) {
    if (pq_full(q)) {
        return false;
    }
    if (pq_empty(q)) {
        q->array[0] = index_create(author, dist);
        q->size += 1;
        return true;
    }
    q->array[pq_size(q)] = index_create(author, dist); //create struct with parameters
    q->size += 1; //enqueue struct at the end of the array and increment the size of it

    for (uint32_t i = 1; i < pq_size(q); i += 1) {
        uint32_t j = i;
        PQindex temp = q->array[i];
        while (j > 0 && temp.dist < q->array[j - 1].dist) {
            q->array[j] = q->array[j - 1];
            j -= 1;
        }
        q->array[j] = temp;
    }
    return true;
    //using insertion sort method
}

bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (pq_empty(q)) {
        return false;
    }
    *author = q->array[0].author;
    *dist = q->array[0].dist;
    for (uint32_t i = 1; i < pq_size(q); i += 1) {
        q->array[i - 1] = q->array[i];
        if (i == pq_size(q)) {
            q->array[i].author = NULL;
            q->array[i].dist = 0;
        }
    }
    q->size -= 1;
    return true;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}

void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < pq_size(q); i += 1) {
        printf("index author = %s, dist = %.15f\n", q->array[i].author, q->array[i].dist);
    }
}
