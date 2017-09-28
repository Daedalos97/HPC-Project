#include "search.h"
#include "lattice.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <getopt.h>
#include <ctype.h>

#define OPTLIST "p:sb"
bool pflag = false;
bool sflag = false;
bool bflag = false;

bool percolates = false;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	int opt;
	while ((opt = getopt(argc,argv,OPTLIST)) != -1) {
		switch (opt) {
			case 'p': 
				if (isdigit(optarg[0])) {
					prob = strtod(optarg,NULL);
					pflag = true;
				} else {
					fprintf(stderr, "Invalid probability argument.\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 's':
				sflag = true;
				break;
			case 'b':
				bflag = true;
				break;
			case '?': fprintf(stderr, "Invalid command.\n"); exit(EXIT_FAILURE);
		}
	}
	if (sflag == bflag) {
		fprintf(stderr, "You need only one -s or -b flag for either site or bond percolation.\n");
		exit(EXIT_FAILURE);
	} else if (!pflag) {
		fprintf(stderr, "No percolation probability given.\n");
		exit(EXIT_FAILURE);
	}

	init_lattice(64);

	//Check if we are looking for the site percolation or bond.
	if (sflag) {
		seed_lattice_sites(prob);
	} else {
		seed_lattice_bonds(prob);
	}

	search_lattice();

	print_lattice(lat.len,'v');

	if (percolates) {
		printf("Percolates at %4.8f\n", prob);
	} else {
		printf("Does not percolate\n");
	}

	exit(EXIT_SUCCESS);
}

