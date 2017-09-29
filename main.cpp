#include "lattice.h"
#include "algorithms.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

const double SITEPROB = 0.4;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	//double percolation_probablity = atof(argc[1]);
	init_lattice(4);
	seed_lattice_sites(SITEPROB);
	int** l = get_lattice_array();
	print_lattice(4, 's');
	perform_depth_first_search(l, 4);
	destroy_lattice(); 
	return 0;
}

