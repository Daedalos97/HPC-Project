#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

typedef struct lattice {
	int len;
	char** lattice_array;
} LATTICE;

extern void init_lattice(int l);
extern void destroy_lattice();
extern void print_lattice(int l, char c);
extern void seed_lattice_sites(double sp);
extern void seed_lattice_bonds(double bp);
extern char** get_lattice_array();

#endif
