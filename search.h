#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>

typedef struct node {
	int position[2];
} NODE;

extern int matchtype;

void search_lattice();
void check_cluster();
void check_cluster_linear();

#endif
