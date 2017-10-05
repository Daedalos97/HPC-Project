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

void explore(int i, int j) {
	printf("Exploring: %d  %d\n", i,j);
	#pragma omp critical
	{
		node_sum++;
		lat.bond_array[i][j].visited = 2;
	//Doesnt Matter if we have a conflict as its set to 1.
	horiz[j] = 1;
	verti[i] = 1;
	if (sflag) {
		printf("Going to explore %d %d\n", (i+lat_size-1)%lat_size,j);
		if (lat.bond_array[(i+lat_size-1)%lat_size][j].visited == 1) {
			#pragma omp task untied
			{
				explore((i+lat_size-1)%lat_size,j);
			}
		}
		printf("Going to explore %d %d\n", i,(j+lat_size-1)%lat_size);
		if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
			#pragma omp task untied
			{
				explore(i,(j+lat_size-1)%lat_size);
			}
		}
		printf("Going to explore %d %d\n", i,(j+1)%lat_size);
		if (lat.bond_array[i][(j+1)%lat_size].visited == 1) {
			#pragma omp task untied
			{
				explore(i,(j+1)%lat_size);
			}
		}
		printf("Going to explore %d %d\n", (i+1)%lat_size,j);
		if (lat.bond_array[(i+1)%lat_size][j].visited == 1) {
			#pragma omp task untied
			{
				explore((i+1)%lat_size,j);
			}
		}
		#pragma omp taskwait
	}
}
}

void check_cluster() {
	largest_cluster = 0;
	node_sum = 0;
	for (int k = 0; k < lat_size; k++) {
		for (int l = 0; l < lat_size; l++) {
			printf("%d %d\n", k,l);
			if (lat.bond_array[k][l].visited == 1) {
				node_sum = 0;

				int horiz_sum = 0;
				int verti_sum = 0;
				for (int i = 0; i < lat_size; i++) {
					horiz[i] = 0;
					verti[i] = 0;
				}

						//Prioritise Down and Right Movement.
						//Site Percolation
					#pragma omp parallel num_threads(4) shared(largest_cluster,node_sum,horiz,verti,lat) firstprivate(lat_size)
					{
						#pragma omp single
							if (sflag) {
								#pragma omp task
								{
									explore(k,l);
								}
							
							}
					} 
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