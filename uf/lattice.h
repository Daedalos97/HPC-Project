#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdbool>

typedef struct lattice {
	int len;
	int** lattice_array;
} LATTICE;

extern void init_lattice(int l);
extern void destroy_lattice();
extern void print_lattice(int l, char c);
extern void seed_lattice_sites(double sp);
extern void seed_lattice_bonds(double bp);
extern int** get_lattice_array();

#endif
