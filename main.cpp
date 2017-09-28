/**
 *	Student: Samuel Heath, Student Number: 21725083
 *	Student: Abrar Amin, Student Number: 21518928
 */
#include <stdbool.h>
#include <ctype.h>
#include <omp.h>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include "lattice.h"

#define SIZE 8
//#define SIZE 22528
#define OPTLIST "p:sb"

bool pflag = false;
bool sflag = false;
bool bflag = false;

bool percolates = false;
double prob;

void seed_sites()
{

}

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

	//Check if we are looking for the site percolation or bond.
	if (sflag) {
		seed_sites();
	} else {
		seed_bonds();
	}

	for (int i = 0; i < SIZE; i++) {
		i = find_start(i);

		if (check_cluster()) {
			percolates = true;
			break;
		}
	}

	if (percolates) {
		printf("Percolates at %4.8f\n", prob);
	} else {
		printf("Does not percolate\n");
	}

	exit(EXIT_SUCCESS);
}
