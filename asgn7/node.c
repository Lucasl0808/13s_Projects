#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

Node *node_create(char *word) {
    Node *n = (Node *) malloc(sizeof(Node));
    char *temp_word = strdup(word);
    n->word = temp_word;
    n->count = 0;
    return n;
}

void node_print(Node *n) {
    printf("Node word = %s\n", n->word);
    printf("Node count = %" PRIu32 "\n", n->count);
}

void node_delete(Node **n) {
    free((*n)->word);
    free(*n);
    *n = NULL;
}
/*
int main(void){
	Node *n = node_create("Hello");
	node_print(n);
	node_delete(&n);
	return 0;
}
*/
