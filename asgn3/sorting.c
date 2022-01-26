#include "stats.h"
#include "batcher.h"
#include "quick.h"
#include "insert.h"
#include "heap.h"
#include <stdio.h>
#include <unistd.h>
#include "set.h"
#include <stdlib.h>
#include <inttypes.h>

int main(int argc, char **argv){
	int opt = 0;
	uint32_t quick = 0; 
	uint32_t heap = 1;
	uint32_t batcher = 2;
	uint32_t insert = 3;
	uint32_t random_seed = 13371453;
	int n = 100; //default size is 100
	int printing = 100; //default print size is 100
	Set args = empty_set();

	while((opt = (getopt(argc, argv, "ahbiqr:n:p:H")) != 1)){
		switch(opt){
		case "a":
			args = insert_set(quick, args);
			args = insert_set(heap, args);
			args = insert_set(batcher, args);
			args = insert_set(insert, args);
			break;
		case "h":
			args = insert_set(heap, args);
			break;
		case "b":
			args = insert_set(batcher, args);
			break;
		case "i":
			args = insert_set(insert, args);
			break;
		case "q":
			args = insert_set(quick, args);
			break;
		case "r":
			random_seed = strtol(optarg, NULL, 10);
			break;
		case "n":
			n = strtol(optarg, NULL, 10);
			break;
		case "p":
			printing = strtol(optarg, NULL, 10);
			break;
		case "H":
			printf("Select at least one sort to perform.\n");
			printf("SYNOPSIS\n");
			printf("	A collection of comparison-based sorting algorithms.\n");
			printf("\nUSAGE\n");
			printf("	./sorting [-Haibhqn:p:r:] [-n length] [-p print_elements] [-r seed]\n");
			printf("OPTIONS\n");
			printf("   -H	Display program help and usage.\n");
			printf("   -a	Enable all sorts.\n");
			printf("   -i	Enable Insertion Sort.\n");
			printf("   -b	Enable Batcher Sort.\n");
			printf("   -h	Enable Heap Sort.\n");
			printf("   -q	Enable Quick Sort.\n");
			printf("   -n length	Specify number of array elements (default: 100)\n");
			printf("   -p elements	Specify number of elements to print (default: 100)\n");
			printf("   -r seed	Specify random seed (default: 13371453)\n");
			break;
		default:
			printf("Select at least one sort to perform.\n");
			printf("SYNOPSIS\n");
			printf("	A collection of comparison-based sorting algorithms.\n");
			printf("\nUSAGE\n");
			printf("	./sorting [-Haibhqn:p:r:] [-n length] [-p print_elements] [-r seed]\n");
			printf("OPTIONS\n");
			printf("   -H	Display program help and usage.\n");
			printf("   -a	Enable all sorts.\n");
			printf("   -i	Enable Insertion Sort.\n");
			printf("   -b	Enable Batcher Sort.\n");
			printf("   -h	Enable Heap Sort.\n");
			printf("   -q	Enable Quick Sort.\n");
			printf("   -n length	Specify number of array elements (default: 100)\n");
			printf("   -p elements	Specify number of elements to print (default: 100)\n");
			printf("   -r seed	Specify random seed (default: 13371453)\n");
			break;

		}

	}
	srandom(random_seed);

	Stats insert_stats;
	Stats quick_stats;
	Stats batcher_stats;
	Stats heap_stats;

	reset(&insert_stats);
	reset(&quick_stats);
	reset(&batcher_stats);
	reset(&heap_stats);

	uint32_t *A = (uint32_t *)malloc(n * sizeof(unint32_t));
	for(int i = 0; i < n; i += 1){
		A[i] = random();
	}
	if(member_set(insert, args)){
		insertion_sort(&insert_stats, A, n); 
		printf("Insertion Sort, %d elements, %lu moves, %lu compares\n", n, insert_stats.moves, insert_stats.compares);
		if(printing == 0){
			return 0;
		}
		if(printing < n){
			for(int i = 0; i < n; i += 1){
				printf("%13" PRIu32, A[i]);
				if( (i+1) % 5 == 0){
					printf("\n");
				}
			}
		}
		if(n >= printing){
			for(int i = 0; i < printing; i+=1){
				printf("%13" PRIu32, A[i]);
				if((i+1) % 5 == 0){
					printf("\n");
				}
			}
		}
	}
	free(*A);
}	

