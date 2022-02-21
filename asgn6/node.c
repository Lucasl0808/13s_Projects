#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"

Node *node_create(uint8_t symbol, uint64_t frequency){
	Node *n = (Node *)malloc(sizeof(Node));
	n->symbol = symbol;
	n->frequency = frequency;
	n->left = NULL;
	n->right = NULL;
	return n;
}

void node_delete(Node **n){
	free(*n);
	*n = NULL;
}

//fix node delete

Node *node_join(Node *left, Node *right){
	Node *p = (Node *)malloc(sizeof(Node));
	p->symbol = '$';
	p->frequency = ((left->frequency) + (right->frequency));
	p->left = NULL;
	p->right = NULL;
	return p;
}

void node_print(Node *n){
	printf("node symbol %c\n", n->symbol);
	printf("node frequency %"PRIu64"\n", n->frequency);
}
/*
int main(void){
	Node *n = node_create('a', 20);
	node_print(n);
	Node *f = node_create('b', 25);
	Node *p = node_join(n, f);
	node_print(p);
	node_delete(&n);
	node_delete(&f);
	node_delete(&p);
}
*/
