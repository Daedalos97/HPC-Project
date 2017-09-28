#include "stack.h"
#include "lattice.h"

#include <stdlib.h>
#include <cstdio>
#include <omp.h>

void search_lattice();

int find_start(int i);

int* find_cluster(int* pos);

bool check_cluster();