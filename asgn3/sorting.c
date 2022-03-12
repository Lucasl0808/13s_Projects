#include "batcher.h"
#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t quick = 0;
    uint32_t heap = 1;
    uint32_t batcher = 2;
    uint32_t insert = 3;
    uint32_t random_seed = 13371453;
    int n = 100; //default size is 100
    int printing = 100; //default print size is 100
    Set args = empty_set();

    while ((opt = (getopt(argc, argv, "ahbiqr:n:p:H"))) != -1) {
        switch (opt) {
        case 'a':
            args = insert_set(quick, args);
            args = insert_set(heap, args);
            args = insert_set(batcher, args);
            args = insert_set(insert, args);
            break;
        case 'h': args = insert_set(heap, args); break;
        case 'b': args = insert_set(batcher, args); break;
        case 'i': args = insert_set(insert, args); break;
        case 'q': args = insert_set(quick, args); break;
        case 'r': random_seed = strtol(optarg, NULL, 10); break;
        case 'n': n = strtol(optarg, NULL, 10); break;
        case 'p': printing = strtol(optarg, NULL, 10); break;
        case 'H':
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
            return 1;
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

    uint32_t *A = (uint32_t *) malloc(n * sizeof(uint32_t));
    for (int i = 0; i < n; i += 1) {
        A[i] = (random() & (1 << 30) - 1); //Masking to fit 30 bits
    }
    if (member_set(insert, args)) {
        insertion_sort(&insert_stats, A, n);
        printf("Insertion Sort, %d elements, %lu moves, %lu compares\n", n, insert_stats.moves,
            insert_stats.compares);
        if (printing == 0) {
            printf("\n");
        }
        if (printing <= n) {
            for (int i = 0; i < printing; i += 1) {
                printf("%13" PRIu32, A[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
        if (printing > n) {
            for (int i = 0; i < n; i += 1) {
                printf("%13" PRIu32, A[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
    }
    free(A);

    srandom(random_seed);
    uint32_t *B = (uint32_t *) malloc(n * sizeof(uint32_t));
    for (int i = 0; i < n; i += 1) {
        B[i] = (random() & (1 << 30) - 1); //Masking to fit 30 bits
    }
    if (member_set(batcher, args)) {
        batcher_sort(&batcher_stats, B, n);
        printf("Batcher Sort, %d elements, %lu moves, %lu compares\n", n, batcher_stats.moves,
            batcher_stats.compares);
        if (printing == 0) {
            printf("\n");
        }
        if (printing <= n) {
            for (int i = 0; i < printing; i += 1) {
                printf("%13" PRIu32, B[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
        if (printing > n) {
            for (int i = 0; i < n; i += 1) {
                printf("%13" PRIu32, B[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
    }
    free(B);

    srandom(random_seed);
    uint32_t *C = (uint32_t *) malloc(n * sizeof(uint32_t));
    for (int i = 0; i < n; i += 1) {
        C[i] = (random() & (1 << 30) - 1); //Masking to fit 30 bits
    }
    if (member_set(heap, args)) {
        heap_sort(&heap_stats, C, n);
        printf("Heap Sort, %d elements, %lu moves, %lu compares\n", n, heap_stats.moves,
            heap_stats.compares);
        if (printing == 0) {
            printf("\n");
        }
        if (printing <= n) {
            for (int i = 0; i < printing; i += 1) {
                printf("%13" PRIu32, C[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
        if (printing > n) {
            for (int i = 0; i < n; i += 1) {
                printf("%13" PRIu32, C[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
    }
    free(C);

    srandom(random_seed);
    uint32_t *D = (uint32_t *) malloc(n * sizeof(uint32_t));
    for (int i = 0; i < n; i += 1) {
        D[i] = (random() & (1 << 30) - 1); //Masking to fit 30 bits
    }
    if (member_set(quick, args)) {
        quick_sort(&quick_stats, D, n);
        printf("Quick Sort, %d elements, %lu moves, %lu compares\n", n, quick_stats.moves,
            quick_stats.compares);
        if (printing == 0) {
            printf("\n");
        }
        if (printing <= n) {
            for (int i = 0; i < printing; i += 1) {
                printf("%13" PRIu32, D[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
        if (printing > n) {
            for (int i = 0; i < n; i += 1) {
                printf("%13" PRIu32, D[i]);
                if ((i + 1) % 5 == 0) {
                    printf("\n");
                }
            }
        }
    }
    free(D);
    return 1;
}
