#include "stats.h"

#include <inttypes.h>
#include <stdio.h>

void insertion_sort(Stats *insert_stats, uint32_t A[], int n) {
    for (int i = 1; i < n; i += 1) {
        int j = i;
        uint32_t temp = move(insert_stats, A[i]);
        while (j > 0 && cmp(insert_stats, temp, A[j - 1]) < 0) {
            A[j] = move(insert_stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(insert_stats, temp);
    }
}
