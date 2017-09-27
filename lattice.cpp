#include "lattice.h"
#include <cstdio>
#include <cstdlib>

LATTICE lat;
int local_len = 8;

/**
 * Prints the existing lattice, takes the length of the 2D array 
 * as argument. If 'v' is passed in as print type a visual representation is 
 * provided, any other character defaults to 1s and 0s. 
 */
void print_lattice(int len, char viewType)
{
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len; j++)
		{
			if(viewType == 'v' || viewType == 'V')
			{
				if(lat.lattice_array[i][j] == 1)
					printf("*");
				else if(lat.lattice_array[i][j] == 2)
					printf("\u2588");
				else
					printf(" ");
			}
			else
				printf("%i", lat.lattice_array[i][j]);
		}
		printf("\n");
	}
}

/**
 * A function that seeds the lattice
 * */
void seed_lattice(double siteProbability)
{
	
}


/**
 * Initialize the lattice struct to specified length.
 * Dynamically allocates the array representing the lattice to a specified size.
 */
void init_lattice(int len)
{
	local_len = len;
	if(len <= 1){
		fprintf(stderr, "%d is an invalid lattice size. Must be greater than 1", len);
		return;
	}
	lat.lattice_dimension = len;
	lat.lattice_array = (int**) malloc(len*sizeof(int*));
	for(int i = 0; i < len; i++){
		lat.lattice_array[i] = (int*) malloc(len*sizeof(int));
	}
}


/**
 * Frees memory.
 * TO DO 
 */
void destroy_lattice()
{

}
