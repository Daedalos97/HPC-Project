#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

typedef struct lattice {
	int len;
	int** lattice_array;
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
