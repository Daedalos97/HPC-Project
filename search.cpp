#include "search.h"

#include <stack>
#include <vector>

int largest_cluster;
std::stack<NODE> S;
int node_sum;
char* horiz = (char*) malloc(lat_size*sizeof(char));
char* verti = (char*) malloc(lat_size*sizeof(char));

void search_lattice() {
	check_cluster();
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

void dfs(int i0, int i1) {
	int size = i1-i0;
	std::stack<NODE> stack;
	for (int k = 0; k < i1; k++) {
		for (int l = 0; l < lat_size; l++) {
			if (lat.bond_array[i][j].visited == 1) {
				node_sum = 0;
				NODE root = {k,l};
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
					if (i >= i0 && i <i1) {
						if (sflag) {
							if (lat.bond_array[(i+size-1)%size][j].visited == 1) {
								NODE new_node = {(i+size-1)%size,j};
								lat.bond_array[(i+size-1)%size][j].visited = 2;
								stack.push(new_node);
							}
							if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
								stack.push(new_node);
							}
							if (lat.bond_array[(i+1)%size][j].visited == 1) {
								NODE new_node = {(i+1)%size,j};
								lat.bond_array[(i+1)%size][j].visited = 2;
								stack.push(new_node);
							}
							if (lat.bond_array[i][(j+1)%lat_size].visited == 1) {
								NODE new_node = {i,(j+1)%lat_size};
								lat.bond_array[i][(j+1)%lat_size].visited= 2;
								stack.push(new_node);	
							} else {
								//Bond Percolation
								BOND b = lat.bond_array[i][j];
								if (b.up && lat.bond_array[(i+size-1)%size][j].visited != 1) {
									NODE new_node = {(i+size-1)%size,j};
									lat.bond_array[(i+size-1)%size][j].visited = 2;
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
								if (b.down && lat.bond_array[(i+1)%size][j].visited != 1) {
									NODE new_node = {(i+1)%size,j};
									lat.bond_array[(i+1)%size][j].visited = 2;
									stack.push(new_node);
								}
							}
						}
					}
				}
			}
		}
	}
}


void check_cluster() {
	largest_cluster = 0;
	node_sum = 0;
	int arr_split = size/omp_get_num_threads();
	#pragma omp parallel num_threads(4) shared(node_sum,horiz,verti,lat) firstprivate(lat_size,largest_cluster)
	{
		int th = omp_get_thread_num();
		dfs(arr_split*th,arr_split*(th+1));
						//Prioritise Down and Right Movement.
						//Site Percolation
					
					/*else {
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
						}*/

					for (int m = 0; m < lat_size; m++) {
						if (horiz[m]) {
							horiz_sum++;
							horiz[m] = 0;
						}
						if (verti[m]) {
							verti_sum++;
							verti[m] = 0;
						}
					}
					//Shouldnt need critical as all other threads are suspended temporarily.
					if (node_sum > largest_cluster) {
						largest_cluster = node_sum;
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