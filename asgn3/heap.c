#include "stats.h"

#include <stdbool.h>
#include <stdio.h>

int max_child(Stats *heap_stats, uint32_t A[], int first, int last) {
    int left = 2 * first;
    int right = left + 1;
    if (right <= last && cmp(heap_stats, A[right - 1], A[left - 1]) > 0) {
        return right;
    }
    return left;
}

void fix_heap(Stats *heap_stats, uint32_t A[], int first, int last) {
    bool found = false;
    int mother = first;
    int great = max_child(heap_stats, A, mother, last);
    while (mother <= last / 2 && found != true) {
        if (cmp(heap_stats, A[mother - 1], A[great - 1]) < 0) {
            swap(heap_stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(heap_stats, A, mother, last);

        } else {
            found = true;
        }
    }
}

void build_heap(Stats *heap_stats, uint32_t A[], int first, int last) {
    for (int father = last / 2; first - 1 < father; father -= 1) {
        fix_heap(heap_stats, A, father, last);
    }
}

void heap_sort(Stats *heap_stats, uint32_t A[], int n) {
    int first = 1;
    int last = n;
    build_heap(heap_stats, A, first, last);
    for (int leaf = last; first < leaf; leaf -= 1) {
        swap(heap_stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(heap_stats, A, first, leaf - 1);
    }
}
