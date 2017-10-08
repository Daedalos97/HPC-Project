#ifndef WEIGHTED_QUICK_UNION_UNION_FIND_H
#define WEIGHTED_QUICK_UNION_UNION_FIND_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <algorithm>
#include <vector>
#include <omp.h>
#include "lattice.h"

extern void init_qu_union_find(int s);
extern void destroy_qu_union_find();
extern void print_array_parentid();
extern void print_array_subtrees();
extern int perform_union_find(int** l, int s);
extern int perform_union_find_multi_threaded(int** l, int s);
extern int perform_union_find_m_t_2(int** l, int s);
extern int perform_union_find_bond(BOND** b, int** l, int s);
extern int perform_union_find_m_t_bond(BOND** b, int** l, int s);
extern bool find_vertical_percolation(int** l, int s);
extern bool find_horizontal_percolation(int** l, int s);
extern void perform_hoshen_kopelman_alg(int** l, int s);

#endif
