#include "stats.h"
#include <stdio.h>
#include "quick.h"
void printarray(uint32_t A[], int n){
	for(int i = 0; i < n; i += 1){
		printf("%d ", A[i]);
	}
}

int main(void){
	/*Stats record;
	reset(&record);
	uint32_t A[5] = {2,3,4,5,6};
	if (cmp(&record, A[4], A[2]) < 0){
		printf("4 < 2\n");
		printf("%lu\n", record.compares);
	}
	else{
		printf("4 > 2");

	}
	*/
	Stats quick_stats;
	reset(&quick_stats);
	uint32_t B[5] = {4,7,1,8,5};
	quick_sort(&quick_stats, B, 5);
	printarray(B, 5);
	printf("moves = %lu\n compares = %lu\n",quick_stats.moves, quick_stats.compares); 
	return 1;
}
