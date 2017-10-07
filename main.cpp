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

#define OPTLIST "p:sbl:t:"
bool pflag = false;
bool sflag = false;
bool bflag = false;
bool lflag = false;
bool percflag = false;

int matchtype = 2;
int lat_size = 4;



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

void start_search() {
	//while (lat_size <= 256) {
		init_lattice();
		//Check if we are looking for the site percolation or bond.
		if (sflag) {
			seed_lattice_sites(prob);
		} else {
			seed_lattice_bonds(prob);
		}
		//print_lattice(lat_size,'v');
		search_lattice();
		//print_lattice(lat_size,'v');
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
	int lsiz;
	while ((opt = getopt(argc,argv,OPTLIST)) != -1) {
		switch (opt) {
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
			case 't':
				if (isdigit(optarg[0])) {
					matchtype = atoi(optarg);
					percflag = true;
				} else {
					fprintf(stderr, "Invalid percolation type argument enter a number between 0 and 2.\n");
				}
				break;
			case '?': fprintf(stderr, "Invalid command.\n"); print_usage(); exit(EXIT_FAILURE);
		}
	}
	if (matchtype < 0 && matchtype > 2) {
		fprintf(stderr, "Invalid Percolation Type Chosen.\n");
		exit(EXIT_FAILURE);
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
	start_search();
	exit(EXIT_SUCCESS);
}
