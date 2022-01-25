#include <stdio.h>
#include "stats.h"

void comparator(Stats *batcher_stats, uint32_t A[], int x, int y){
	if(cmp(batcher_stats, A[x], A[y]) > 0){
		swap(batcher_stats, &A[x], &A[y]);
	}
}

void batcher_sort(Stats *batcher_stats, uint32_t A[], int n){
	if(n == 0){
		return;
	}
	int count = 0;
	int temp_n = n;
	while(temp_n > 0){
		temp_n /= 2;
		count += 1;
	}
	int t = count;
	int p = 1 << (t - 1);
	while(p > 0){
		int q = 1 << (t - 1);
		int r = 0;
		int d = p;
		
		while( d > 0){
			for(int i =0; i < n-d; i += 1){
				if((i & p) == r){
					comparator(batcher_stats, A, i, i +d);
				}
			}
			d = q - p;
			q = q >> 1;
			r = p;	
		}
		p = p >> 1;
	}

}
