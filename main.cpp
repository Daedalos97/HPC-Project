/**
 *	Student: Samuel Heath, Student Number: 21725083
 *	Student: Abrar Amin, Student Number: 21518928
 */
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

int lat_size = 4;

void start_search() {
	//while (lat_size <= 256) {
		init_lattice();
		//Check if we are looking for the site percolation or bond.
		if (sflag) {
			seed_lattice_sites(prob);
		} else {
			seed_lattice_bonds(prob);
		}
		search_lattice();
		print_lattice(lat.len,'v');
		//lat_size = lat_size * 2;
	//}
}

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

	start_search();

	exit(EXIT_SUCCESS);
}