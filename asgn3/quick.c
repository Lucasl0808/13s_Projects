#include <stdio.h>
#include "stats.h"

Stats quick_stats;

int partition(uint32_t A[], int low, int high){
	int i = low -1;
	for(int j = low; j < high, j ++){
		if(cmp(&quick_stats,A[j - 1], A[high - 1]) < 0){
			i += 1;
			int temp = A[i - 1];
			A[i - 1] = A[j - 1];
			A[j - 1] = temp;
		}
	}
	swap(&quick_stats, A[i], A[high -1]);

	//int temp2 = A[i];
	//A[i] = A[high - 1];
	//A[high  - 1] = temp2;
	return i + 1;
}

void quick_sorter(uint32_t A[], int low, int high){
	if(cmp(&quick_stats,low, high) < 0){
		int p = partition(A, low, high);
		quick_sorter(A, low, p - 1);
		quick_sorter(A, p + 1, high);
	}
}

void quick_sort(Stats *stats, uint32_t A[], int n){
	quick_sorter(A, 1, n);
}
