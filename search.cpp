#include "search.h"

void search_lattice() {
	for (int i = 0; i < lat.len; i++) {
		i = find_start(i);

		if (check_cluster()) {
			break;
		}
	}
}

//Allows us to find bonds on th edge of the lattice.
int find_start(int last_tried) {
	for (int i = last_tried; i < lat.len; i++) {
		if (lat.lattice_array[0][i] == 1) {
			lat.lattice_array[0][i] = 2; //visited.
			NODE n = {0,i};
			push(n);
			return i;
		}
		if (lat.lattice_array[i][0] == 1) {
			lat.lattice_array[i][0] = 2; //visited.
			NODE n = {i,0};
			push(n);
			return i;
		}
	}
	return lat.len;
}

/**
 *	Looks for the starting point of a cluster
 */
int* find_cluster(int* last_pos) {
	for (int i = last_pos[0]; i < lat.len-1; i+=2) {
		for (int j = last_pos[1]; j < lat.len-1; j+=2) {
			if (lat.lattice_array[i][j] == 1) {
				lat.lattice_array[i][j] = 2;
				last_pos[0] = i, last_pos[1] = j;
				NODE n = {i,j};
				push(n);
				return last_pos;
			}
		}
	}
	last_pos[0] = lat.len; last_pos[1] = lat.len;
	return last_pos;
}

void addNode(int i, int j) {
	NODE n = {i,j};
	push(n);
}

bool check_cluster() {
	int horiz_sum = 0;
	int verti_sum = 0;
	int horiz[lat_size];
	int verti[lat_size]; 
	while (!is_empty()) {
		NODE* n = pop();

		int i = n->position[0],j = n->position[1];
		if (horiz[j] == 0) {
			horiz[j] = 1;
			horiz_sum++;
		}
		if (verti == 0) {
			verti[i] = 1;
			verti_sum++;
		}
		if (lat.lattice_array[(i+1)%lat_size][j] == 1) addNode(i+1%lat_size,j);
		if (lat.lattice_array[i][(j+1)%lat_size] == 1) addNode(i,(j+1)%lat_size);
		if (lat.lattice_array[(i+lat_size-1)%lat_size][j] == 1) addNode((i+lat_size-1)%lat_size,j);
		if (lat.lattice_array[i][(j+lat_size-1)%lat_size] == 1) addNode(i, (j+lat_size-1)%lat_size);
	}
	if (horiz_sum == lat.len && verti_sum == lat.len) {
		printf("Percolates Horizontally & Vertically!\n");
		return true;
	} else if (horiz_sum == lat.len) {
		printf("Percolates Horizontally!\n");
		return true;
	} else if (verti_sum == lat.len) {
		printf("Percolates Vertically!\n");
		return true;
	}
	printf("%d\t%d\n",horiz_sum,verti_sum);
	return false;
}