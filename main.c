#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <omp.h>
#include <getopt.h>

#define SIZE 4096
#define OPTLIST "p:sb"

bool pflag = false;
bool sflag = false;
bool bflag = false;

double prob;

typedef struct lattice {
	bool sites[SIZE][SIZE];
} LATTICE;

LATTICE lattice;

void seed_sites() {
	#pragma omp parallel for num_threads(8) collapse(2)
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if ((double) (rand()/(RAND_MAX+1.0)) < prob) {
					lattice.sites[i][j] = true;
				}
			}
		}
}

void seed_bonds () {
	#pragma omp parallel for num_threads(8) collapse(2)
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {

			}

		}
}

void find_bonds() {

}



int main(int argc, char *argv[]) {
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

	if (sflag) {
		seed_sites();
	} else {
		seed_bonds();
	}

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%d ", lattice.sites[i][j]);
		}
		printf("\n");
	}

	


	exit(EXIT_SUCCESS);
}