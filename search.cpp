#include "search.h"

#include <stack>
#include <vector>

int largest_cluster;
std::stack<NODE> S;
int node_sum;
char* horiz = (char*) malloc(lat_size*sizeof(char));
char* verti = (char*) malloc(lat_size*sizeof(char));

void search_lattice() {
	check_cluster_linear();
	printf("Largest Cluster is %d nodes\n", largest_cluster);
}

void check_cluster_linear() {
	std::stack<NODE> stack;
	char* horiz = (char*) malloc(lat_size*sizeof(char));
	char* verti = (char*) malloc(lat_size*sizeof(char));
	largest_cluster = 0;
	for (int k = 0; k < lat_size; k++) {
		for (int l = 0; l < lat_size; l++) {
			if (lat.bond_array[k][l].visited == 1) {
				node_sum = 0;
				int horiz_sum = 0;
				int verti_sum = 0;
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
							lat.bond_array[i][(j+1)%lat_size].visited = 2;
							stack.push(new_node);	
						}
					}
					if (bflag) {
						//Bond Percolation
						BOND b = lat.bond_array[i][j];
						if (b.up == 1 && lat.bond_array[(i+lat_size-1)%lat_size][j].visited != 2) {
							NODE new_node = {(i+lat_size-1)%lat_size,j};
							lat.bond_array[(i+lat_size-1)%lat_size][j].visited = 2;
							stack.push(new_node);
						}
						if (b.left == 1 && lat.bond_array[i][(j+1)%lat_size].visited != 2) {
							NODE new_node = {i,(j+1)%lat_size};
							lat.bond_array[i][(j+1)%lat_size].visited = 2;
							stack.push(new_node);	
						}
						if (b.right == 1 && lat.bond_array[i][(j+lat_size-1)%lat_size].visited != 2) {
						 	NODE new_node = {i, (j+lat_size-1)%lat_size};
						 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
							stack.push(new_node);
						}
						if (b.down == 1 && lat.bond_array[(i+1)%lat_size][j].visited != 2) {
							NODE new_node = {(i+1)%lat_size,j};
							lat.bond_array[(i+1)%lat_size][j].visited = 2;
							stack.push(new_node);
						}
					}
				}
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

void dfs(SUB_LAT sub) {
	int i0 = sub.x;
	int i1 = sub.x1;
	int horiz_sum = 0;
	int verti_sum = 0;
	int cluster_count = 0;
	int thread = omp_get_thread_num();
	printf("Thread %d: Limits %d %d\n", thread,i0,i1);
	for (int k = i0; k < i1; k++) {
		for (int l = 0; l < lat_size; l++) {
			if (sub.bonds[k][l].visited == 1) {
				//printf("Thread %d: %d %d\n", thread,k,l);
				sub.clust[k][l] = cluster_count;
				node_sum = 0;
				NODE root = {k,l};
				sub.bonds[k][l].visited = 2;
				S.push(root);

				while (!S.empty()) {
					NODE node = S.top();
					node_sum++;
					int i = node.position[0],j = node.position[1];
					//printf("Thread %d: JUST BEFORE SEG\n", thread);
					S.pop();
					//printf("Thread %d: JUST AFTER\n",thread);
					if (!horiz[j]) {
						horiz[j] = 1;
						horiz_sum++;
					}
					if (!verti[i]) {
						verti[i] = 1;
						verti_sum++;
					}
					if (i > i0+1 && i < i1-1) {
						if (sflag) {
							//printf("Thread %d: %d %d\n",thread, (i-1),j);
							if (i-1 >= i0 && sub.bonds[i-1][j].visited == 1) {
								NODE new_node = {i-1,j};
								sub.bonds[i-1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread, i,(j+lat_size-1)%lat_size);
							if (sub.bonds[i][(j+lat_size-1)%lat_size].visited == 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	sub.bonds[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n",thread, i+1,j);
							if (sub.bonds[i+1][j].visited == 1) {
								NODE new_node = {i+1,j};
								sub.bonds[i+1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread,i,(j+1)%lat_size);
							if (sub.bonds[i][(j+1)%lat_size].visited == 1) {
								NODE new_node = {i,(j+1)%lat_size};
								sub.bonds[i][(j+1)%lat_size].visited= 2;
								S.push(new_node);
							} /*else {
								//Bond Percolation
								BOND b = lat.bond_array[i][j];
								if (b.up && lat.bond_array[(i+size-1)%size][j].visited != 1) {
									NODE new_node = {(i+size-1)%size,j};
									lat.bond_array[(i+size-1)%size][j].visited = 2;
									S.push(new_node);
								}
								if (b.left && lat.bond_array[i][(j+1)%lat_size].visited != 1) {
									NODE new_node = {i,(j+1)%lat_size};
									lat.bond_array[i][(j+1)%lat_size].visited = 2;
									S.push(new_node);	
								}
								if (b.right && lat.bond_array[i][(j+lat_size-1)%lat_size].visited != 1) {
								 	NODE new_node = {i, (j+lat_size-1)%lat_size};
								 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
									S.push(new_node);
								}
								if (b.down && lat.bond_array[(i+1)%size][j].visited != 1) {
									NODE new_node = {(i+1)%size,j};
									lat.bond_array[(i+1)%size][j].visited = 2;
									S.push(new_node);
								}
							}*/
						}
					}
				}
				sub.clust_size[cluster_count] = node_sum;
			}
		}
	}
}

//Adds the sub lat to lat.bond_array
void combine_lats(SUB_LAT sub) {
	for (int i = sub.x; i < sub.x1; i++) {
		memcpy(&lat.bond_array[i], &sub.bonds[i], lat_size*sizeof(BOND));
		free(sub.bonds[i]);
	}
}

//Combine sections on the bottom of the sections of work
void combine(int* max, char* perc, int bot_row) {
	for (int i = 0; i < lat_size; i++) {
		if (lat.bond_array[bot_row][i].visited == 1 && lat.bond_array[bot_row][(i+1)%lat_size].visited) {
			//combine that cluster with the other
		}
	}
}

//Splits the larger lattice into sub lattices avoiding race conditions
SUB_LAT create_sub_lat(int i0, int i1) {
	SUB_LAT sub;
	sub.x = i0;
	sub.x1 = i1;
	sub.bonds = (BOND**) malloc(sizeof(BOND*));
	sub.clust = (int**) malloc(sizeof(int*));
	sub.clust_size = (int*) malloc(sizeof(int));
	for (int k = i0; k < i1; k++) {
		sub.clust[k] = (int*) malloc(sizeof(int));
		sub.bonds[k] = (BOND*) malloc(sizeof(BOND));
		for (int l = 0; l < lat_size; l++)
			memcpy(&sub.bonds[k][l], &lat.bond_array[k][l], sizeof(BOND));
	}
	return sub;
}

void check_cluster() {
	largest_cluster = 0;
	char percolates = 0; // false
	node_sum = 0;
	int arr_split = lat_size/8;
	printf("%d\n", arr_split);
	#pragma omp parallel num_threads(8) shared(horiz,verti) firstprivate(node_sum)
	{
		int th = omp_get_thread_num();
		SUB_LAT sub = create_sub_lat(arr_split*th,arr_split*(th+1)); // Creates sub lattice, then DFS it and combine with actual lattice
		dfs(sub);
		combine_lats(sub);
	}

	for (int i = 1; i < 8; i++) {
		combine(&largest_cluster, &percolates, (i*arr_split)-1);
	}


}
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
						}

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
					}*/