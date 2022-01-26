#include "stats.h"

#include <stdio.h>

//Stats quick_stats;

int partition(Stats *quick_stats, uint32_t A[], int low, int high) {
    int i = low - 1;
    for (int j = low; j < high; j += 1) {
        if (cmp(quick_stats, A[j - 1], A[high - 1]) < 0) {
            i += 1;
            swap(quick_stats, &A[i - 1], &A[j - 1]);
            //int temp = A[i - 1];
            //A[i - 1] = A[j - 1];
            //A[j - 1] = temp;
        }
    }
    swap(quick_stats, &A[i], &A[high - 1]);

    //int temp2 = A[i];
    //A[i] = A[high - 1];
    //A[high  - 1] = temp2;
    return i + 1;
}

void quick_sorter(Stats *quick_stats, uint32_t A[], int low, int high) {
    if (low < high) {
        int p = partition(quick_stats, A, low, high);
        quick_sorter(quick_stats, A, low, p - 1);
        quick_sorter(quick_stats, A, p + 1, high);
    }
}

void quick_sort(Stats *quick_stats, uint32_t A[], int n) {
    quick_sorter(quick_stats, A, 1, n);
}
