#include "search.h"
#include <vector>

std::stack<NODE> stack;
int perc_type;

bool percolates = false;
bool end = false;
int largest_cluster = 0;

//Parallelised Variables made global
int node_sum;
int horiz_sum;
int verti_sum;
std::vector<int> horiz;
std::vector<int> verti;

void search_lattice() {
	int* pos = (int*) malloc(2*sizeof(int));
	pos[0] = 0, pos[1] = 0;
	while (!end) {
		pos = find_cluster(pos);
		NODE n = {pos[0],pos[1]};
		if (check_cluster(n)) {
			percolates = true;
			break; //We just want to know if its possible
		}
	}
	if (percolates) {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	} else {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	}
	end = false;
}

/**
 *	Looks for the starting point of a cluster
 */
int* find_cluster(int* last_pos) {
	for (; last_pos[0] < lat.len; last_pos[0]++) {
		for (; last_pos[1] < lat.len; last_pos[1] += 2) {
			if (lat.lattice_array[last_pos[0]][last_pos[1]] == 1) {
				lat.lattice_array[last_pos[0]][last_pos[1]] = 2;
				return last_pos;
			}
		}
		last_pos[1] = 0; //stops the column from staying on the last column or past the len of lattice
	}
	end = true;
	last_pos[0] = lat.len; last_pos[1] = lat.len;
	return last_pos;
}

void get_neighbours(int i, int j, std::vector<NODE>* v) {
	#pragma omp critical
	{
		if (lat.lattice_array[(i+1)%lat_size][j] == 1) {
			NODE new_node = {(i+1)%lat_size,j};
			v->push_back(new_node);
			lat.lattice_array[(i+1)%lat_size][j] = 2;
		}
		if (lat.lattice_array[i][(j+1)%lat_size] == 1) {
			NODE new_node = {i,(j+1)%lat_size};
			v->push_back(new_node);
			lat.lattice_array[i][(j+1)%lat_size] = 2;
		}
		if (lat.lattice_array[(i+lat_size-1)%lat_size][j] == 1) {
			NODE new_node = {(i+lat_size-1)%lat_size,j};
			v->push_back(new_node);
			lat.lattice_array[(i+lat_size-1)%lat_size][j] = 2;
		}
		if (lat.lattice_array[i][(j+lat_size-1)%lat_size] == 1) {
		 	NODE new_node = {i, (j+lat_size-1)%lat_size};
		 	v->push_back(new_node);
		 	lat.lattice_array[i][(j+lat_size-1)%lat_size] = 2;
		}
	}
}

//Recursive Parallelisation of the Stack
void explore(NODE n) {
	int i = n.position[0], j = n.position[1];
	#pragma omp atomic
		node_sum++;
	#pragma omp critical
	{
		if (horiz.at(j) == 0) {
			horiz.at(j) = 1;
		}
		if (verti.at(i) == 0) {
			verti.at(i) = 1;
		}
	}
	std::vector<NODE> neighbours (0);
	get_neighbours(i, j, &neighbours);
	for (int k = 0; k < (int)neighbours.size(); k++) {
		#pragma omp task 
		{
			explore(neighbours.at(k));
		}
	}
}

bool check_cluster(NODE n) {
	if (n.position[0] == lat.len || n.position[1] == lat.len) return false;
	node_sum = 0;
	horiz_sum = 0;
	verti_sum = 0;
	horiz.clear();
	horiz.resize(lat.len);
	verti.clear();
	verti.resize(lat.len);
	#pragma omp parallel
	{
		#pragma omp single
		{
			explore(n);
		}
	}
	int i;
	#pragma omp parallel for reduction(+:horiz_sum,verti_sum)
		for (i = 0; i < lat.len; i++) {
			if (horiz.at(i)) {
				#pragma omp atomic
					horiz_sum++;
			}
			if (verti.at(i)) {
				#pragma omp atomic
					verti_sum++;
			}
		}

	if (node_sum > largest_cluster) largest_cluster = node_sum;
	if (horiz_sum == lat.len && verti_sum == lat.len && perc_type == 2) {
		printf("Percolates Horizontally & Vertically!\n");
		return true;
	} else if (horiz_sum == lat.len && perc_type == 1) {
		printf("Percolates Horizontally!\n");
		return true;
	} else if (verti_sum == lat.len && perc_type == 0) {
		printf("\nPercolates Vertically!\n");
		return true;
	} else {
		//printf("%d %d\n", horiz_sum, verti_sum);
		return false;
	}
}