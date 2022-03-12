#include "universe.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t i = 0; i < rows; i += 1) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }
    return u;
}

void uv_delete(Universe *u) {
    for (uint32_t i = 0; i < u->rows; i += 1) {
        free(u->grid[i]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r > uv_rows(u) - 1 || c > uv_cols(u) - 1) {
        return;
    } else if ((int) r < 0 || (int) c < 0) {
        return;
    } else {
        u->grid[r][c] = true;
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r > uv_rows(u) - 1 || c > uv_cols(u) - 1) {
        return;
    } else if ((int) r < 0 || (int) c < 0) {
        return;
    } else {
        u->grid[r][c] = false;
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r > uv_rows(u) || c > uv_cols(u)) {
        return false;
    } else if ((int) r < 0 || (int) c < 0) {
        return false;
    } else {
        return u->grid[r][c];
    }
}

bool uv_populate(Universe *u, FILE *infile) {
    //File is open and starts at the second line.
    uint32_t row;
    uint32_t col;
    while (fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", &row, &col) != -1) {
        if (row > uv_rows(u) || col > uv_cols(u)) {
            return false;
        }
        if ((int) row < 0 || (int) col < 0) {
            return false;
        }
        uv_live_cell(u, row, col);
    }
    fclose(infile);
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t counter = 0;
    if (u->toroidal == false) {
        if ((int) (r - 1) >= 0) { //checks above
            if (u->grid[r - 1][c] == true) {
                counter += 1;
            }
        }
        if ((int) (r + 1) <= (int) (uv_rows(u) - 1)) { //checks below
            if (u->grid[r + 1][c] == true) {
                counter += 1;
            }
        }
        if ((int) (c - 1) >= 0) { //checks left
            if (u->grid[r][c - 1] == true) {
                counter += 1;
            }
        }
        if ((int) (c + 1) <= (int) (uv_cols(u) - 1)) { //checks right
            if (u->grid[r][c + 1] == true) {
                counter += 1;
            }
        }
        if (((int) (r - 1) >= 0) && ((int) (c - 1) >= 0)) { //checks top left
            if (u->grid[r - 1][c - 1] == true) {
                counter += 1;
            }
        }
        if (((int) (r - 1) >= 0) && ((int) (c + 1) <= (int) (uv_cols(u) - 1))) { //checks top right
            if (u->grid[r - 1][c + 1] == true) {
                counter += 1;
            }
        }
        if (((int) (r + 1) <= (int) (uv_rows(u) - 1))
            && ((int) (c - 1) >= 0)) { //checks bottom left
            if (u->grid[r + 1][c - 1] == true) {
                counter += 1;
            }
        }
        if (((int) (r + 1) <= (int) (uv_rows(u) - 1))
            && ((int) (c + 1) <= (int) (uv_cols(u) - 1))) { //checks bottom right
            if (u->grid[r + 1][c + 1] == true) {
                counter += 1;
            }
        }

        return counter;
    }
    if (u->toroidal == true) {
        uint32_t above_row = ((r + uv_rows(u) - 1) % uv_rows(u));
        uint32_t below_row = ((r + uv_rows(u) + 1) % uv_rows(u));
        uint32_t left_col = ((c + uv_cols(u) - 1) % uv_cols(u));
        uint32_t right_col = ((c + uv_cols(u) + 1) % uv_cols(u));
        if (u->grid[above_row][left_col] == true) {
            counter += 1;
        }
        if (u->grid[above_row][c] == true) {
            counter += 1;
        }
        if (u->grid[above_row][right_col] == true) {
            counter += 1;
        }
        if (u->grid[r][left_col] == true) {
            counter += 1;
        }
        if (u->grid[r][right_col] == true) {
            counter += 1;
        }
        if (u->grid[below_row][left_col] == true) {
            counter += 1;
        }
        if (u->grid[below_row][c] == true) {
            counter += 1;
        }
        if (u->grid[below_row][right_col] == true) {
            counter += 1;
        }
        return counter;
    }
    return 0;
}

void uv_print(Universe *u, FILE *outfile) {
    //outfile is open prior to calling function
    for (uint32_t i = 0; i < uv_rows(u); i += 1) {
        for (uint32_t j = 0; j < uv_cols(u); j += 1) {
            if (uv_get_cell(u, i, j)) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
    fclose(outfile);
}
