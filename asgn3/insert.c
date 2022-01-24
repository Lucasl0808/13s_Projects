#include <stdio.h>
#include "stats.h"

Stats insert_stats;
reset(insert_stats);
void insertion_sort(Stats *stats, uint32_t A[], int n){
	for(int i = 1; i < n; i++){
		int j = i;
		int temp = A[i];
		while( j > 0 && cmp(&insert_stats, temp, A[j-1]) < 0){
			A[j] = move(&insert_stats, A[j -1];
			j -= 1;
		}
		A[j] = move(&insert_stats, temp);
	}
}


