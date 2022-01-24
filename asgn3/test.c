#include "stats.h"
#include <stdio.h>

int main(void){
	Stats record;
	reset(&record);
	uint32_t A[5] = {2,3,4,5,6};
	if (cmp(&record, A[4], A[2]) < 0){
		printf("4 < 2\n");
		printf("%lu\n", record.compares);
	}
	else{
		printf("4 > 2");

	}
	return 1;
}
