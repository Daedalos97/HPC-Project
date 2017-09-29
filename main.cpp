#include "lattice.h"
#include "algorithms.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

const double SITEPROB = 0.5;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	//double percolation_probablity = atof(argc[1]);
	init_lattice(16);
	seed_lattice_sites(SITEPROB);
	print_lattice(16, 'v');
	destroy_lattice(); //unimplemented..
	return 0;
}

