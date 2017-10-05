#include "lattice.h"

LATTICE lat;
double prob;
int lat_siz;
/**
 * Prints the existing lattice, takes the length of the 2D array
 * as argument. If 'v' is passed in as print type a visual representation is
 * provided, any other character defaults to 1s and 0s.
 */
void print_lattice(int len, char viewType)
{
	if (!bflag) {
		for(int i = 0; i < len; i++)
		{
			for(int j = 0; j < len; j++)
			{
				if(viewType == 'v' || viewType == 'V')
				{
					if(lat.bond_array[i][j].visited == 1)
						printf("*");
					else if(lat.bond_array[i][j].visited == 2)
						printf("\u2588");
					else
						printf(" ");
				}
				else
					printf("%i", lat.bond_array[i][j].visited);
			}
			printf("\n");
		}
	} else {
		for (int i = 0; i < len; i++) {
			for (int j = 0; j < len; j++) {
				if(viewType == 'v' || viewType == 'V') {
					BOND b = lat.bond_array[i][j];
					if (b.visited == 2) printf("\u2588");
					else if (b.visited == 1) printf("*");
					else printf(" ");
				} else printf("%i", 0);
			}
			printf("\n");
		}
	}
}

/**
 * A function that seeds the lattice sites, provided a site probability.
 */
void seed_lattice_sites(double prob)
{
	//seeding pseudo-random number generator.
	srand(time(NULL));
	#pragma omp parallel for collapse(2) num_threads(8)
		for(int i = 0; i < lat_size; i++) {
			for(int j = 0; j < lat_size; j++) {
				double site_prob = (double)rand()/(double)RAND_MAX;
				if(site_prob < prob )
					lat.bond_array[i][j].visited = 1;
				else
					lat.bond_array[i][j].visited = 0;
			}
		}
}


/**
 * A function that seeds the lattice bonds, provided a bond probability.
 */
void seed_lattice_bonds(double prob)
{
	srand(time(NULL));
	for(int i = 0; i < lat_size; i++) {
		for(int j = 0; j < lat_size; j++) {
			double bond_prob = (double)rand()/(double)RAND_MAX;
			double bond_prob1 = (double)rand()/(double)RAND_MAX;
			if(bond_prob < prob) {
				lat.bond_array[i][j].down = 1;
				lat.bond_array[(i+1)%lat_size][j].up = 1; //down neighbour
				//Set Below unvisited
				lat.bond_array[i][j].visited = 1;
				lat.bond_array[(i+1)%lat_size][j].visited = 1;
			}
			if(bond_prob1 < prob) {
				lat.bond_array[i][j].right = 1;
				lat.bond_array[i][(j+1)%lat_size].left = 1; //right neighbour
				//Set below to unvisited
				lat.bond_array[i][j].visited = 1;
				lat.bond_array[i][(j+1)%lat_size].visited = 1;
			}
		}
	}
}

/**
 * Initialize the lattice struct to specified length.
 * Dynamically allocates the array representing the lattice to a specified size.
 */
void init_lattice()
{
	if(lat_size <= 1){
		fprintf(stderr, "%d is an invalid lattice size. Must be greater than 1", lat_size);
		return;
	}
	lat.bond_array = (BOND**) malloc(lat_size*sizeof(BOND*));
	for(int i = 0; i < lat_size; i++){
		lat.bond_array[i] = (BOND*) malloc(lat_size*sizeof(BOND));
	}
}


/**
 * Frees memory.
 * TO DO
 */
void destroy_lattice()
{
	for (int i = 0; i < lat_size; i++) {
		free(lat.bond_array[i]);
	}
	free(lat.bond_array);
}
