#pragma once

#include "stats.h"

int max_child(Stats *stats, int first, int last);

void fix_heap(Stats *stats, int first, int last);

void build_heap(Stats *stats, int first, int last);

void heap_sort(Stats *stats, uint32_t *A, uint32_t n);
