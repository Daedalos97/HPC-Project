#include <stdbool.h>

#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

extern bool bflag;

typedef struct bond {
	unsigned char left:2,up:2,right:2,down:2;
} BOND;

typedef struct lattice {
	int len;
	char** lattice_array;
	BOND** bond_array;
} LATTICE;

extern LATTICE lat;
extern int lat_size;
extern double prob;

extern void init_lattice();
extern void destroy_lattice();
extern void print_lattice(int l, char c);
extern void seed_lattice_sites(double sp);
extern void seed_lattice_bonds(double bp);

#endif
