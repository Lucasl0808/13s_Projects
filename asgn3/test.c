#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//This code is for testing each function individually before going into sorting.c
//This is also for testing the malloc command to initialize arrays
void printarray(uint32_t A[], int n) {
    for (int i = 0; i < n; i += 1) {
        printf("%d ", A[i]);
    }
}

int main(void) {
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
    srandom(13371453);
    Stats insert_stats;
    reset(&insert_stats);
    uint32_t *A = (uint32_t *) malloc(15 * sizeof(uint32_t));
    for (int i = 0; i < 15; i += 1) {
        A[i] = (random() & (1 << 30) - 1);
    }
    insertion_sort(&insert_stats, A, 15);
    printarray(A, 15);
    printf("insertion sort\nmoves = %lu\n compares = %lu\n", insert_stats.moves,
        insert_stats.compares);
    srandom(13371453);
    Stats quick_stats;
    reset(&quick_stats);
    uint32_t *B = (uint32_t *) malloc(15 * sizeof(uint32_t));
    for (int i = 0; i < 15; i += 1) {
        B[i] = (random() & (1 << 30) - 1);
    }
    quick_sort(&quick_stats, B, 15);
    printarray(B, 15);
    printf("quicksort\nmoves = %lu\n compares = %lu\n", quick_stats.moves, quick_stats.compares);

    srandom(13371453);
    Stats heap_stats;
    reset(&heap_stats);
    uint32_t *C = (uint32_t *) malloc(15 * sizeof(uint32_t));
    for (int i = 0; i < 15; i += 1) {
        C[i] = (random() & (1 << 30) - 1);
    }
    heap_sort(&heap_stats, C, 15);
    printarray(C, 15);
    printf("heapsort\nmoves = %lu\n compares = %lu\n", heap_stats.moves, heap_stats.compares);

    srandom(13371453);
    Stats batcher_stats;
    reset(&batcher_stats);
    uint32_t *D = (uint32_t *) malloc(15 * sizeof(uint32_t));
    for (int i = 0; i < 15; i += 1) {
        D[i] = (random() & (1 << 30) - 1);
    }
    batcher_sort(&batcher_stats, D, 15);
    printarray(D, 15);
    printf(
        "batchersort\nmoves = %lu\n compares = %lu\n", batcher_stats.moves, batcher_stats.compares);

    for (int i = 0; i < 15; i += 1) {
        printf("%13" PRIu32, D[i]);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    free(A);
    free(B);
    free(C);
    free(D);

    return 1;
    //To run test, clang -Wall -Werror -Wpedantic -Wextra -o test test.c quick.c heap.c stats.c -lm
}
