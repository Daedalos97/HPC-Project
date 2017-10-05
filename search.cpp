#include "search.h"

#include <stack>
#include <vector>

int largest_cluster;

void search_lattice() {
	check_cluster_linear();
	printf("Largest Cluster is %d nodes\n", largest_cluster);
}

void check_cluster_linear() {
	int node_sum;
	std::stack<NODE> stack;
	for (int k = 0; k < lat_size; k++) {
		for (int l = 0; l < lat_size; l++) {
			if (lat.bond_array[k][l].visited == 1) {
				node_sum = 0;
				int horiz_sum = 0;
				int verti_sum = 0;
				char* horiz = (char*) malloc(lat_size*sizeof(char));
				char* verti = (char*) malloc(lat_size*sizeof(char));
				for (int i = 0; i < lat_size; i++) {
					horiz[i] = 0;
					verti[i] = 0;
				}

				NODE root = {k,l};
				lat.bond_array[k][l].visited = 2;
				stack.push(root);

				while (!stack.empty()) {
					NODE node = stack.top();
					node_sum++;
					int i = node.position[0],j = node.position[1];
					stack.pop();
					if (!horiz[j]) {
						horiz[j] = 1;
						horiz_sum++;
					}
					if (!verti[i]) {
						verti[i] = 1;
						verti_sum++;
					}
					if (sflag) {
						if (lat.bond_array[(i+lat_size-1)%lat_size][j].visited == 1) {
							NODE new_node = {(i+lat_size-1)%lat_size,j};
							lat.bond_array[(i+lat_size-1)%lat_size][j].visited = 2;
							stack.push(new_node);
						}
						if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
						 	NODE new_node = {i, (j+lat_size-1)%lat_size};
						 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
							stack.push(new_node);
						}
						if (lat.bond_array[(i+1)%lat_size][j].visited == 1) {
							NODE new_node = {(i+1)%lat_size,j};
							lat.bond_array[(i+1)%lat_size][j].visited = 2;
							stack.push(new_node);
						}
						if (lat.bond_array[i][(j+1)%lat_size].visited == 1) {
							NODE new_node = {i,(j+1)%lat_size};
							lat.bond_array[i][(j+1)%lat_size].visited= 2;
							stack.push(new_node);	
						} else {
							//Bond Percolation
							BOND b = lat.bond_array[i][j];
							if (b.up && lat.bond_array[(i+lat_size-1)%lat_size][j].visited != 1) {
								NODE new_node = {(i+lat_size-1)%lat_size,j};
								lat.bond_array[(i+lat_size-1)%lat_size][j].visited = 2;
								stack.push(new_node);
							}
							if (b.left && lat.bond_array[i][(j+1)%lat_size].visited != 1) {
								NODE new_node = {i,(j+1)%lat_size};
								lat.bond_array[i][(j+1)%lat_size].visited = 2;
								stack.push(new_node);	
							}
							if (b.right && lat.bond_array[i][(j+lat_size-1)%lat_size].visited != 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
								stack.push(new_node);
							}
							if (b.down && lat.bond_array[(i+1)%lat_size][j].visited != 1) {
								NODE new_node = {(i+1)%lat_size,j};
								lat.bond_array[(i+1)%lat_size][j].visited = 2;
								stack.push(new_node);
							}
						}
					}
				}
				free(horiz);
				free(verti);
				if (node_sum > largest_cluster) largest_cluster = node_sum;
				if (horiz_sum == lat_size && verti_sum == lat_size && matchtype == 2) {
					printf("Percolates Horizontally & Vertically!\n");
				} else if (horiz_sum == lat_size && matchtype == 1) {
					printf("Percolates Horizontally!\n");
				} else if (verti_sum == lat_size && matchtype == 0) {
					printf("\nPercolates Vertically!\n");
				} else {
				}
			}
		}
	}
}

void check_cluster() {
	largest_cluster = 0;
	int k,l;
	#pragma omp parallel num_threads(4) shared(largest_cluster)
	{
		std::stack<NODE> S;
		#pragma omp for
			for (k = 0; k < lat_size; k++) {
				for (l = 0; l < lat_size; l++) {
					if (lat.bond_array[k][l].visited == 1) {
						char* horiz = (char*) malloc(lat_size*sizeof(char));
						char* verti = (char*) malloc(lat_size*sizeof(char));
						
						NODE n = {k,l};
						lat.bond_array[k][l].visited = 2;
						S.push(n);

						int node_sum = 0;
						int horiz_sum = 0;
						int verti_sum = 0;

						while (!S.empty()) {
							
							NODE new_node = S.top();
							int i = new_node.position[0], j = new_node.position[1];
							horiz[i] = 1;
							verti[j] = 1;

							S.pop();
							node_sum++;

							if (horiz[j]) {
								horiz_sum++;
							}
							if (verti[i]) {
								verti_sum++;
							}

							//Prioritise Down and Right Movement by pushing this last.
							//Site Percolation
							if (sflag) {
								if (lat.bond_array[(i+lat_size-1)%lat_size][j].visited == 1) {
									lat.bond_array[(i+lat_size-1)%lat_size][j].visited = 2;
									NODE neigh = {(i+lat_size-1)%lat_size,j};
									S.push(neigh);
								}
								if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
									lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
									NODE neigh = {i,(j+lat_size-1)%lat_size};
									S.push(neigh);
								}
								if (lat.bond_array[i][(j+1)%lat_size].visited == 1) {
									lat.bond_array[i][(j+1)%lat_size].visited = 2;
									NODE neigh = {i,(j+1)%lat_size};
									S.push(neigh);
								}
								if (lat.bond_array[(i+1)%lat_size][j].visited == 1) {
									lat.bond_array[(i+1)%lat_size][j].visited = 2;
									NODE neigh = {(i+1)%lat_size,j};
									S.push(neigh);	
								}
							} else {
								//Bond Percolation
								BOND b = lat.bond_array[i][j];
								if (b.up == 1 && lat.bond_array[(i+lat_size-1)%lat_size][j].visited == 1) {
									NODE new_node = {(i+lat_size-1)%lat_size,j};
									lat.bond_array[(i+lat_size-1)%lat_size][j].visited = 2;
									S.push(new_node);
								}
								if (b.left == 1 && lat.bond_array[i][(j+1)%lat_size].visited == 1) {
									NODE new_node = {i,(j+1)%lat_size};
									lat.bond_array[i][(j+1)%lat_size].visited = 2;
									S.push(new_node);	
								}
								if (b.right == 1 && lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
								 	NODE new_node = {i, (j+lat_size-1)%lat_size};
								 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
									S.push(new_node);
								}
								if (b.down == 1 && lat.bond_array[(i+1)%lat_size][j].visited == 1) {
									NODE new_node = {(i+1)%lat_size,j};
									lat.bond_array[(i+1)%lat_size][j].visited = 2;
									S.push(new_node);
								}
							}
						}
						free(horiz);
						free(verti);
						#pragma omp critical 
						{
							if (node_sum > largest_cluster) {
								largest_cluster = node_sum;
							}
						}
						if (horiz_sum == lat_size && verti_sum == lat_size && matchtype == 2) {
							printf("Percolates Horizontally & Vertically!\n");
						} else if (horiz_sum == lat_size && matchtype == 1) {
							printf("Percolates Horizontally!\n");
						} else if (verti_sum == lat_size && matchtype == 0) {
							printf("\nPercolates Vertically!\n");
						}
					}
				}
			}
	}
}