#include "wqu_unionfind.h"
#include "lattice.h"
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <cstdbool>
#include <getopt.h>
#include <ctype.h>

#define OPTLIST "p:l:sb"

bool pflag = false;
bool sflag = false;
bool bflag = false;
bool lflag = false;

int match_type = 2;
int lat_size = 2048;
double prob = 0.5;


// compile project:
// g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
// g++ -std=c++11 -Wall -Werror -pedantic -c  wqu_unionfind.cpp -fopenmp
// g++ -std=c++11 -Wall -Werror -pedantic -fopenmp main.cpp -o percolation
// Alternatively you can use the provided Makefile.

/**
 * Incase of invalid command line input from user, print usage.
 */
void print_usage()
{
	FILE* fp;
	fp = fopen(".usage.in", "r");
	if(fp == NULL)
	{
		fprintf(stderr, "ERROR! Could not print usage\n");
		exit(1);
	}

	char c = fgetc(fp);
	while(c != EOF)
	{
		fprintf(stderr, "%c", c);
		c = fgetc(fp);
	}
	fclose(fp);
}



void start_union_find()
{
	init_lattice();
	if(!bflag)
		seed_lattice_sites(prob);
	else
		seed_lattice_bonds(prob);
	int** l = get_lattice_array();
	//print_lattice(lat_size, 's');
	int early_exit;
	init_qu_union_find(lat_size);
	if(!bflag)
		early_exit = perform_union_find_multi_threaded(l, lat_size); //check if all cols percolate
	else
	{
		BOND** b = get_bond_array();
		early_exit = perform_union_find_bond(b, l, lat_size);
	}
	perform_hoshen_kopelman_alg(l, lat_size);
	//print_lattice(lat_size, 's');
	if(early_exit != -1)
	{
		//find_horizontal_percolation(l);
		if(match_type == 0)
			find_vertical_percolation(l, lat_size);
		else if(match_type == 1)
			find_horizontal_percolation(l, lat_size);
		else
		{
			find_vertical_percolation(l, lat_size);
			find_horizontal_percolation(l, lat_size);
		}
	}
	destroy_qu_union_find();
	destroy_lattice();
}



int main(int argc, char** argv)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	int opt;
	int lsiz;

	while ((opt = getopt(argc,argv,OPTLIST)) != -1)
	{
		switch (opt)
		{
			case 'p':
				if (isdigit(optarg[0])) {
					prob = strtod(optarg,NULL);
					pflag = true;
				} else {
					fprintf(stderr, "Invalid probability argument.\n");
					print_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'l':
				if (isdigit(optarg[0])){
					lsiz = atoi(optarg);
					lflag = true;
				} else {
					fprintf(stderr, "Invalid lattice size.\n");
					print_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 's':
				sflag = true;
				break;
			case 'b':
				bflag = true;
				break;
			case '?': fprintf(stderr, "Invalid command.\n"); print_usage(); exit(EXIT_FAILURE);
		}
	}
	if (sflag == bflag) {
		fprintf(stderr, "You need only one -s or -b flag for either site or bond percolation.\n");
		print_usage();
		exit(EXIT_FAILURE);
	} else if (!pflag) {
		fprintf(stderr, "No percolation probability given.\n");
		print_usage();
		exit(EXIT_FAILURE);
	}
	if(lflag)
		lat_size = lsiz;
	start_union_find();
	gettimeofday(&end, NULL);
	double delta = ((end.tv_sec - start.tv_sec) * 1000000u +
							end.tv_usec - start.tv_usec) / 1.e6;
	printf("---Time = %12.10fs---\n", delta);
	return 0;
}
