#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "stack.h"
#include "huffman.h"
#include "pq.h"

Node *build_tree(uint64_t hist[static ALPHABET]) {
    //uses given symbols and frequencies to construct huffman tree
    //index of the array hist = symbol
    //value at hist[index] = frequency
    //create nodes for each symbol (index) and set frequency to value at array

    //capacity of priority queue = length of histogram
    PriorityQueue *q = pq_create(ALPHABET);

    for (int i = 0; i < ALPHABET; i += 1) { //check each index of histogram
        if (hist[i] > 0) { //if frequency of symbol is > 0
            Node *n = node_create(i, hist[i]); //make a node of symbol(ASCII) and freq
            enqueue(q, n); //enqueue node n into priority queue
        }
    }
    //queue is constructed and sorted with lowest freq in index 0
    while (pq_size(q) > 1) { //while there is more than 1 node priority queue (not root node)
        Node *left = NULL; //initialize left Node
        Node *right = NULL; //initialize right Node
        dequeue(q, &left);
        dequeue(q, &right); //dequeue 2 nodes from q to get left and right nodes
        Node *parent = node_join(left, right); //parent node = joined frequencies of left and right
        enqueue(q, parent); //enqueue parent node into priority queue
    }
    Node *root = NULL;
    dequeue(q, &root); //store root node into root
    pq_delete(&q); //q is empty so we can delete it
    return root;
}

Code c = { 0 }; //initialize empty code

void build_codes(Node *root, Code table[static ALPHABET]) {
    //table = array of codes, length 256 (ALPHABET)
    //run through the huffman tree and assign codes to each symbol(index in array)
    //code is initialized before
    //static Code c;

    if (root != NULL) {
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c; //table at index symbol = current code
        } else {
            uint8_t temp = 0;
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &temp);

            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &temp);
        }
    }
}

void dump_tree(int outfile, Node *root) {
    //post order traverse tree by writing L with symbol of leaf, or I for interior node
    if (root != NULL) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if (root->left == NULL && root->right == NULL) { //writes leaf node to outfile
            write(outfile, "L", 1);
            write(outfile, &(root->symbol), 1);

        } else {
            write(outfile, "I", 1); //writes Interior node to outfile without symbol
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    //construct huffman tree using dumped tree (dump_tree) stored in tree_dump
    //return root of tree
    //tree dump = array of Leaf and interior nodes with symbols
    Stack *s = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i += 1) {
        if (tree_dump[i] == 'L') { //if current index of the tree dump is an L
            Node *n
                = node_create(tree_dump[i + 1], 1); //create node with next index symbol and 0 freq
            stack_push(s, n); //push node to stack
            i += 1; //read letter after the symbol
        } else if (tree_dump[i] == 'I') { //if current index of tree dump was not an L, and it is I
            Node *l;
            Node *r;
            stack_pop(s, &r); //pop stack to get right child - store it in r
            stack_pop(s, &l); //pop stack to get left child- store it in l
            Node *p = node_join(l, r); //parent = left joined with right
            stack_push(s, p); //push parent node onto stack
        }
    }
    //after iterating through tree_dump there should be one node left in the stack
    Node *root = NULL;
    stack_pop(s, &root);
    stack_delete(&s);
    return root; //pop last element in the stack to give us the root of the tree and then return it
}

void delete_tree(Node **root) {
    //destroy the huffman tree, use post order traversal to free all nodes and set root pointer to null after freeing
    if (*root != NULL) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);

        free(*root);
        *root = NULL;
    }
}
