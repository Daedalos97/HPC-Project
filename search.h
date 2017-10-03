#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>
#include <stack>

typedef struct node {
	int position[2];
} NODE;

extern std::stack<NODE> stack;

void search_lattice();
int find_start(int i);
int* find_cluster(int* pos);
int* find_cluster_bonds(int* pos);
bool check_cluster(NODE n);
bool check_cluster_bonds(NODE n);

#endif
