#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bv.h"

struct BitVector{
	uint32_t length;
	uint8_t *vector;
};

BitVector *bv_create(uint32_t length){
	//parameter length is in bits
	BitVector *bv = (BitVector *)malloc(sizeof(BitVector));
	bv->length = length;
	uint32_t temp_length = length / 8;
	if(length % 8 != 0){
		temp_length += 1;
	}
	printf("temp_length = %"PRIu32"\n", temp_length);
	bv->vector = (uint8_t *)calloc(temp_length, sizeof(uint8_t));
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

bool bv_set_bit(BitVector *bv, uint32_t i){
	//i = bit index in bv->vector
	//i/8 = index in vector
	//i % 8 how many bits to shift
	if(i < 0 || i > bv_length(bv)){
		return false;
	}

	bv->vector[i/8] = bv->vector[i/8] | (0x1 << (i % 8));
	//vector = itself | (vector of 0's except where you are setting the bit where its 1)
	return true;
}


void bv_print(BitVector *bv){
	for(uint32_t i = 0; i < bv->length; i += 1){
		printf("current byte value = %"PRIu8 "\n", bv->vector[i]);
	}
}

int main(void){
	BitVector *bv = bv_create(10);
	bv_set_bit(bv, 0);
	bv_set_bit(bv, 2);
	bv_print(bv);
}
