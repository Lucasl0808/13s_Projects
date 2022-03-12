#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "bv.h"

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    //parameter length is in bits
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv == NULL) {
        return NULL;
    }
    bv->length = length;
    uint32_t temp_length = length / 8;
    if (length % 8 != 0) {
        temp_length += 1;
    }
    bv->vector = (uint8_t *) calloc(temp_length, sizeof(uint8_t));
    return bv;
}

void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

bool bv_set_bit(BitVector *bv, uint32_t i) {
    //i = bit index in bv->vector
    //i/8 = index in vector
    //i % 8 how many bits to shift
    if (i < 0 || i > bv_length(bv)) {
        return false;
    }

    bv->vector[i / 8] = bv->vector[i / 8] | (0x1 << (i % 8));
    //vector = itself | (vector of 0's except where you are setting the bit where its 1)
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i > bv_length(bv)) {
        return false;
    }
    uint8_t temp = 1; //1 = 2^0
    for (uint32_t x = 0; x < (i % 8); x += 1) {
        temp *= 2;
    }
    //temp = 2^bit location
    uint8_t cmp_vector = 255 - temp; //array of 1's except for where to clear the bit
    bv->vector[i / 8] = bv->vector[i / 8] & cmp_vector; //clear the bit of vector
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i < 0 || i > bv_length(bv)) {
        return false;
    }
    uint8_t value = bv->vector[i / 8] >> (i % 8);

    //shifts desired bit into the 0th index
    if (value % 2 == 1) { //if desired bit = 1
        return true;
    } else {
        return false; //if desired bit = 0
    }
}

void bv_print(BitVector *bv) {
    uint32_t temp = bv->length / 8;
    printf("%" PRIu32 "\n", temp);
    /*if(bv->length %8 != 0){
		temp += 1;
	}
	for(uint32_t i = 0; i < temp; i += 1){
		printf("current byte value = %"PRIu8 "\n", bv->vector[i]);
	}*/
}

/*
int main(void){
	BitVector *bv = bv_create(10);
	bv_set_bit(bv, 0);
	bv_set_bit(bv, 2);
	bv_set_bit(bv, 8);
	bv_clr_bit(bv, 8);
	bv_clr_bit(bv, 2);
	bv_set_bit(bv, 5);
	bv_set_bit(bv, 9);
	bv_set_bit(bv, 10);
	if(bv_get_bit(bv, 10)){
		printf("index is 1\n");
	}
	bv_print(bv);
	bv_delete(&bv);
}
*/
