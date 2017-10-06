#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>
#include <string.h>

typedef struct node {
	int position[2];
} NODE;

typedef struct sub_lat
{
	BOND** bonds;
	int x, x1; //The extent of the division of the lattice.
	int** clust;
	int dom_clustid; //The cluster dominating this sublattice e.g. biggest
	int* clust_size;
} SUB_LAT;

extern int matchtype;

void search_lattice();
void check_cluster();
void check_cluster_linear();

#endif
