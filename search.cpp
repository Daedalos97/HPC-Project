#include "search.h"

#include <stack>
#include <vector>

int perc_type;
int largest_cluster;

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
	largest_cluster = 0;
	char stop = 1;
	std::stack<NODE> S;
	int k,l;
	#pragma omp parallel num_threads(16) shared(largest_cluster) firstprivate(S)
		#pragma omp for
			for (k = 0; k < lat_size; k++) {
				for (l = 0; l < lat_size; l++) {
					if (lat.lattice_array[k][l] == 1 && stop) {
						char* horiz = (char*) malloc(lat_size*sizeof(char));
						char* verti = (char*) malloc(lat_size*sizeof(char));
						NODE n = {k,l};
						S.push(n);

						lat.lattice_array[k][l] = 2;

						int node_sum = 0;
						int horiz_sum = 0;
						int verti_sum = 0;

						while (!S.empty() && stop) {
							node_sum++;
							NODE new_node = S.top();
							int i = new_node.position[0], j = new_node.position[1];
							horiz[i] = 1;
							verti[j] = 1;
							S.pop();

							if (lat.lattice_array[(i+lat_size-1)%lat_size][j] == 1) {
								lat.lattice_array[(i+lat_size-1)%lat_size][j] = 2;
								NODE neigh = {(i+lat_size-1)%lat_size,j};
								S.push(neigh);
							}
							if (lat.lattice_array[i][(j+lat_size-1)%lat_size] == 1) {
								lat.lattice_array[i][(j+lat_size-1)%lat_size] = 2;
								NODE neigh = {i,(j+lat_size-1)%lat_size};
								S.push(neigh);
							}
							if (lat.lattice_array[i][(j+1)%lat_size] == 1) {
								lat.lattice_array[i][(j+1)%lat_size] = 2;
								NODE neigh = {i,(j+1)%lat_size};
								S.push(neigh);
							}
							if (lat.lattice_array[(i+1)%lat_size][j] == 1) {
								lat.lattice_array[(i+1)%lat_size][j] = 2;
								NODE neigh = {(i+1)%lat_size,j};
								S.push(neigh);	
							}
							
						}
						for (int j = 0; j < lat.len; j++) {
							if (horiz[j]) {
								horiz_sum++;
							}
							if (verti[j]) {
								verti_sum++;
							}
						}
						#pragma omp critical 
						{
							if (node_sum > largest_cluster) {
								largest_cluster = node_sum;
								printf("%d\n", largest_cluster);
							}
							if (horiz_sum == lat_size && verti_sum == lat_size && perc_type == 2) {
								printf("Percolates Horizontally & Vertically!\n");
								stop = 0;
							} else if (horiz_sum == lat_size && perc_type == 1) {
								printf("Percolates Horizontally!\n");
								stop = 0;
							} else if (verti_sum == lat_size && perc_type == 0) {
								printf("\nPercolates Vertically!\n");
								stop = 0;
							}
						}
					}
				}
			}
	return false;
}