#include "search.h"

#include <stack>
#include <vector>

int perc_type;
int largest_cluster = 0;

void search_lattice() {
	bool percolates = false;
	if (check_cluster()) {
		percolates = true;
	}
	if (percolates) {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	} else {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	}
}

//Paralelised DFS using multiple threads hopefully no errors
bool check_cluster() {
	int k;
	int node_sum;
	int horiz_sum;
	int verti_sum;
	#pragma omp parallel for shared(largest_cluster) private(node_sum,verti_sum,horiz_sum)
		for (k = 0; k < lat_size; k++) {
			for (int l = 0; l < lat_size; l++) {
				if (lat.lattice_array[k][l] == 1) {
					std::vector<int> horiz (lat_size);
					std::vector<int> verti (lat_size);
					std::stack<NODE> stack;
					NODE n = {k,l};
					lat.lattice_array[k][l] = 2;
					stack.push(n);

					while (!stack.empty()) {
						NODE new_node = stack.top();
						int i = new_node.position[0], j = new_node.position[1];
						horiz.at(i) = 1;
						verti.at(j) = 1;
						stack.pop();
						node_sum++;

						if (lat.lattice_array[(i+lat_size-1)%lat_size][j]) {
							lat.lattice_array[(i+lat_size-1)%lat_size][j] = 2;
							NODE neigh = {(i+lat_size-1)%lat_size,j};
							stack.push(neigh);
						}
						if (lat.lattice_array[i][(j+lat_size-1)%lat_size]) {
							lat.lattice_array[i][(j+lat_size-1)%lat_size] = 2;
							NODE neigh = {i,(j+lat_size-1)%lat_size};
							stack.push(neigh);
						}
						if (lat.lattice_array[i][(j+1)%lat_size]) {
							lat.lattice_array[i][(j+1)%lat_size] = 2;
							NODE neigh = {i,(j+1)%lat_size};
							stack.push(neigh);
						}
						if (lat.lattice_array[(i+1)%lat_size][j]) {
							lat.lattice_array[(i+1)%lat_size][j] = 2;
							NODE neigh = {(i+1)%lat_size,j};
							stack.push(neigh);	
						}
						
					}
					for (int j = 0; j < lat.len; j++) {
						if (horiz.at(j)) {
							horiz_sum++;
						}
						if (verti.at(j)) {
							verti_sum++;
						}
					}
				
					if (horiz_sum == lat.len && verti_sum == lat.len && perc_type == 2) {
						printf("Percolates Horizontally & Vertically!\n");
						k = lat_size;
					} else if (horiz_sum == lat.len && perc_type == 1) {
						printf("Percolates Horizontally!\n");
						k = lat_size;
					} else if (verti_sum == lat.len && perc_type == 0) {
						printf("\nPercolates Vertically!\n");
						k = lat_size;
					} else {
						#pragma omp critical
						{
							if (node_sum > largest_cluster) largest_cluster = node_sum;
						}
					}
				}
			}
		}
	return false;
}