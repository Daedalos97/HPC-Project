#include "wqu_unionfind.h"
#include "lattice.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdbool>

const double SITEPROB = 0.4;
const int LATSIZE = 12;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	//double percolation_probablity = atof(argc[1]);
	init_lattice(LATSIZE);
	seed_lattice_sites(SITEPROB);
	int** l = get_lattice_array();
	//print_lattice(LATSIZE, 's');
	init_qu_union_find(LATSIZE);
	//printf("parent IDs\n");
	//print_array_parentid();
	//printf("subtree sizes\n");
	//print_array_subtrees();
	perform_union_find(l, LATSIZE); //check if all cols percolate
	//print_array_parentid();
	//printf("subtrees\n");
	//print_array_subtrees();
	perform_hoshen_kopelman_alg(l, LATSIZE);
	print_lattice(LATSIZE, 's');
	//print_array_parentid();
	//print_array_subtrees();
	//printf("\n");
	//bool check_percolation_row = check_if_percolates_row();
	//printf("**percolates? rows %d\n", check_percolation_row);
	destroy_qu_union_find();
	destroy_lattice();
	return 0;
}
