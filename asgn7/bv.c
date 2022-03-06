#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "bv.h"

struct BitVector{
	uint32_t length;
	uint8_t *vector;
};

BitVector *bv_create(uint32_t length){
	BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
	bv->length = length;
	bv->vector = (uint8_t *)calloc(sizeof(uint8_t) * length);
	//FIND A WAY TO TEST IF THERE IS SUFFICIENT MEMORY TO ALLOCATE
	return bv;
}

void bv_delete(BitVector **bv){
	free((*bv)->vector);
	free(*bv);
	*bv = NULL;
}

uint32_t bv_length(BitVector *bv){
	return bv->length;
}


