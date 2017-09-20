#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <omp.h>
#include <getopt.h>

#define SIZE 64
#define OPTLIST "p:sb"

bool pflag = false;
bool sflag = false;
bool bflag = false;
bool percolates = false;

double prob;

/**
 *	Stack
 */
typedef struct node {
	struct node* parent;
	int position[2];
} NODE;

NODE *stack;
int size_stack = 0;

bool isEmpty() { return !(size_stack > 0); }

NODE* peek() {
	if (!isEmpty()) return &stack[size_stack];
	else return NULL;
}

//Remember to release memory or save elsewhere after you pop.
NODE* pop() {
	if (!isEmpty()) {
		return &stack[--size_stack];
	} else return NULL;
}

void push(NODE n) {
	stack = (NODE *) realloc(stack,sizeof(NODE)*(++size_stack));
	stack[size_stack-1] = n;
}
/**
 * End Stack
 */

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
				if ((double) (rand()/(RAND_MAX+1.0)) < prob) {
					//Creates bonds wit surrounding sites by setting them to occupied
					//NOT SURE IF THIS IS CORRECT!!!
					lattice.sites[i][j] = true;
					lattice.sites[i][j+(1%SIZE)] = true;
					lattice.sites[i+(1%SIZE)][j] = true;
				}
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

	if (percolates) {
		printf("Percolates at %.8f\n", prob);
	} else {
		printf("Does not percolate\n");
	}

	exit(EXIT_SUCCESS);
}