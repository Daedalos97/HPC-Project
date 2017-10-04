#ifndef WEIGHTED_QUICK_UNION_UNION_FIND_H
#define WEIGHTED_QUICK_UNION_UNION_FIND_H

#include <cstdlib>
#include <cstdio>
#include <cstdbool>
#include <algorithm>

extern void init_qu_union_find(int s);
extern void destroy_qu_union_find();
extern void print_array_parentid();
extern void print_array_subtrees();
extern void perform_union_find(int** l, int s);
// extern bool check_if_percolates_row();
// extern bool check_if_percolates_col();
extern void perform_hoshen_kopelman_alg(int** l, int s);

#endif
