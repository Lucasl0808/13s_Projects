#include "huffman.h"
#include "pq.h"

Node *build_tree(uint64_t hist[static ALPHABET]){
	//uses given symbols and frequencies to construct huffman tree
	//index of the array hist = symbol
	//value at hist[index] = frequency
	//create nodes for each symbol (index) and set frequency to value at array
	
	//capacity of priority queue = length of histogram
	PriorityQueue *q = pq_create(ALPHABET);

	for(int i = 0; i < ALPHABET; i += 1){	//check each index of histogram
		if(hist[i] > 0){	//if frequency of symbol is > 0
			Node *n = node_create(char(i), hist[i]);	//make a node of symbol(ASCII) and freq
			enqueue(q, n);	//enqueue node n into priority queue
		}
	}
	//queue is constructed and sorted with lowest freq in index 0
	
	while(pq_size(q) > 1){	//while there is more than 1 node priority queue (not root node)
		Node *left == NULL;	//initialize left Node
		Node *right = NULL;	//initialize right Node
		dequeue(q, &left);
		dequeue(q, &right);	//dequeue 2 nodes from q to get left and right nodes
		Node *parent = node_join(left, right);	//parent node = joined frequencies of left and right
		enqueue(q, parent);	//enqueue parent node into priority queue
	}
	Node *root = NULL;
	dequeue(q, &root);	//store root node into root
	pq_delete(q);	//q is empty so we can delete it
	return root;
}

void build_codes(Node *root, Code table[static ALPHABET]){
	//table = array of codes, length 256 (ALPHABET)
	
}

void dump_tree(int outfile, Node *root){
	//post order traverse tree by writing L with symbol of leaf, or I for interior node
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]){
	//construct huffman tree using dumped tree (dump_tree) stored in tree_dump
	//return root of tree
}

void delete_tree(Node **root){
	//destroy the huffman tree, use post order traversal to free all nodes and set root pointer to null after freeing
}
