#include "bf.h"
#include "salts.h"

struct BloomFilter{
	uint64_t primary[2];
	uint64_t secondary[2];
	uint64_t tertiary[2];
	BitVector *filter;
};

BloomFilter *bf_create(uint32_t size){
	BloomFilter *bf = (BloomFilter *)malloc(BloomFilter);
	bf->primary[0] = SALT_PRIMARY_LO;
	bf->primary[1] = SALT_PRIMARY_HI;
	bf->secondary[0] = SALT_SECONDARY_LO;
	bf->secondary[1] = SALT_SECONDARY_HI;
	bf->tertiary[0] = SALT_TERTIARY_LO;
	bf->tertiary[1] = SALT_TERTIARY_HI;
	bf->filter = bv_create(size);
	return bf;
}

void bf_delete(BloomFilter **bf){
	bv_delete(&(*bf)->filter);
	free(*bf);
	*bf = NULL;
}

uint32_t bf_size(BloomFilter *bf){
	return bf->filter->length;
}

void bf_insert(BloomFilter *bf, char *word){
	int index1 = hash(bf->primary, word) % bv_length(bf->filter);
	int index2 = hash(bf->secondary, word) % bv_length(bf->filter);
	int index3 = hash(bf->tertiary, word) % bv_length(bf->filter);
	//3 indices for hashing with each salt
	//perform modulus on the hash function to get it to access a specific bit in the bitvector
	bv_set_bit(bf->filter, index1);
	bv_set_bit(bf->filter, index2);
	bv_set_bit(bf->filter, index3);
}

bool bf_probe(BloomFilter *bf, char *word){
	int index1 = hash(bf->primary, word) % bv_length(bf->filter);
	int index2 = hash(bf->secondary, word) % bv_length(bf->filter);
	int index3 = hash(bf->tertiary, word) % bv_length(bf->filter);
	
	if(bv_get_bit(bf->filter, index1) && bv_get_bit(bf->filter, index2) && bv_get_bit(bf->filter, index3)){
		return true;
	}
	else{
		return false;
	}
}

void bf_print(BloomFilter *bf){
	bv_print(bf->filter);
}


