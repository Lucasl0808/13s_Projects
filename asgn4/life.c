#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DELAY 50000

void update(Universe *u, Universe *r) { // takes universe u and r as parameters. Updates universe r using Conway's game of life rules
    for (uint32_t i = 0; i < uv_rows(u); i += 1) {
        for (uint32_t j = 0; j < uv_cols(u); j += 1) {
            uint32_t count = uv_census(u, i, j);
            if (uv_get_cell(u, i, j)) { //if current cell is alive
                if (count < 2 || count > 3) {
                    uv_dead_cell(r, i, j);
                } else {
                    uv_live_cell(r, i, j);
                }
            } else if (uv_get_cell(u, i, j) == false) { //if current cell is dead
                if (count == 3) {
                    uv_live_cell(r, i, j);
                } else {
                    uv_dead_cell(r, i, j);
                }
            }
        }
    }
}

void swap(Universe *u, Universe *r) { // Swaps two Universes
    Universe *c = u;
    u = r;
    r = c;
}

int main(int argc, char **argv) {
    int opt = 0;
    uint32_t generations = 100;
    bool silence = true;
    bool toro = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    while ((opt = (getopt(argc, argv, "tsn:i:o:"))) != -1) {
        switch (opt) {
        case 't': toro = true; break;
        case 's': silence = false; break;
        case 'n': generations = strtol(optarg, NULL, 10); break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        default:
            printf("Input valid entries for Conway's game of life\n");
            printf("Synopsis\n");
            printf("	Simulation for Conway's game of life.\n");
            printf("\nUSAGE\n");
            printf("	./life [-tsn:i:o:] [-n generations] [-i infile] [-o outfile]\n");
            printf("OPTIONS\n");
            printf("   -t	Enable toroidal universe.\n");
            printf("   -s	Silence ncurses window.\n");
            printf("   -n generations	specify number of generations to run through. Default is "
                   "100 generations.\n");
            printf("   -i infile	specify file to create universe. Default is stdin.\n");
            printf("   -o outfile	specify file to print universe. Default is stdout.\n");
            return 1;
        }
    }
    int total_rows;
    int total_cols;
    fscanf(infile, "%d %d\n", &total_rows, &total_cols);
    Universe *A = uv_create(total_rows, total_cols, toro);
    Universe *B = uv_create(total_rows, total_cols, toro);
    uv_populate(A, infile);

    if (generations == 0) { // if the number of generations is specified to 0
        uv_print(A, outfile);
        uv_delete(A);
        uv_delete(B);
        return 0;
    }
    if (silence == true) {
        initscr();
        curs_set(FALSE);
        for (uint32_t x = 0; x < generations; x += 1) {
            clear();
            if ((int) x % 2 == 0) {// Print moving o for even numbered generations
                for (uint32_t row = 0; row < uv_rows(A); row += 1) {
                    for (uint32_t col = 0; col < uv_cols(A); col += 1) {
                        if (uv_get_cell(A, row, col)) {
                            mvprintw(row, col, "o");
                        }
                    }
                }
            }
            if ((int) x % 2 == 1) {// Print moving o for odd numbered generations
                for (uint32_t row = 0; row < uv_rows(A); row += 1) {
                    for (uint32_t col = 0; col < uv_cols(A); col += 1) {
                        if (uv_get_cell(B, row, col)) {
                            mvprintw(row, col, "o");
                        }
                    }
                }
            }
            refresh();
            usleep(DELAY);
            if (x % 2 == 0) {
                update(A, B);
            }
            if (x % 2 == 1) {
                update(B, A);
            }
            swap(A, B);
        }
        endwin();
        if (generations % 2 == 0) {
            uv_print(A, outfile);
        } else {
            uv_print(B, outfile);
        }
        uv_delete(A);
        uv_delete(B);
        return 0;

    } else {
        for (uint32_t i = 0; i < generations; i += 1) {
            if ((int) i % 2 == 0) { //if it is an even generation
                update(A, B);
                swap(A, B);
            }
            if ((int) i % 2 == 1) { //if it is an odd generation
                update(B, A);
                swap(A, B);
            }
        }
        if (generations % 2 == 0) {
            uv_print(A, outfile);
        } else {
            uv_print(B, outfile);
        }

        uv_delete(A);
        uv_delete(B);
        return 0;
    }
}
