#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>
#include <string.h>
#include <vector>

typedef struct node {
	int position[2];
} NODE;

struct sub_lat
{
	int x, x1; //The extent of the division of the lattice.
	int** clust;
	std::vector<std::vector<NODE>> clusters; //keeps list of all clusters of a lattice
	short* top_edge; //Maps top edge of area of work to a cluster
	short* bot_edge; //Maps bot edge of area of work to a cluster
};

extern int matchtype;

void search_lattice();
void check_cluster();
void check_cluster_linear();

#endif
