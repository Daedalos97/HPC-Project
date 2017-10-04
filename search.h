#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>

typedef struct node {
	int position[2];
} NODE;

extern int perc_type;

void search_lattice();
bool check_cluster();