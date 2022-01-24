#include <stdio.h>
#include "stats.h"
#include <inttypes.h>


void insertion_sort(Stats *insert_stats, uint32_t A[], int n){
	for(int i = 1; i < n; i++){
		int j = i;
		int temp = A[i];
		while( j > 0 && cmp(insert_stats, temp, A[j-1]) < 0){
			A[j] = move(insert_stats, A[j -1]);
			j -= 1;
		}
		A[j] = move(insert_stats, temp);
	}
}

void printarray(uint32_t A[],int n){
	for(int i = 0; i < n; i+= 1){
		printf("%u", A[i]);
	}
}
int main(void){
	uint32_t A[5] = {4, 1, 7, 3, 8};
	Stats insert_stats;
	reset(&insert_stats);
	insertion_sort(&insert_stats, A, 5);
	printarray(A, 5);
	printf("moves = %lu", insert_stats.moves);
	printf("compares = %lu",insert_stats.compares);
	return 1;
}


