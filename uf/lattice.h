#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdbool>


typedef struct bond {
	unsigned char left:1,up:1,right:1,down:1,visited:2; //Uses 6 bits out of the 8
} BOND;

typedef struct lattice {
	int len;
	int** lattice_array;
	BOND** bond_array;
} LATTICE;

extern int lat_size;
extern bool bflag;

extern void init_lattice();
extern void destroy_lattice();
extern void print_lattice(int l, char c);
extern void seed_lattice_sites(double p);
extern void seed_lattice_bonds(double p);
extern int** get_lattice_array();
extern BOND** get_bond_array();

#endif
