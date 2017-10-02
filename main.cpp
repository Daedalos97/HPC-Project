#include "lattice.h"
#include "algorithms.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

const double SITEPROB = 0.5;
const int LATSIZE = 1024;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	//double percolation_probablity = atof(argc[1]);
	init_lattice(LATSIZE);
	seed_lattice_sites(SITEPROB);
	char** l = get_lattice_array();
	//print_lattice(LATSIZE, 's');
	perform_depth_first_search(l, LATSIZE);
	destroy_lattice(); 
	return 0;
}

