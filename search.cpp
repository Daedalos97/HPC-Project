#include "search.h"

#include <stack>

int largest_cluster;
//std::stack<NODE> S;
int node_sum;

void search_lattice() {
	check_cluster();
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
						if (b.left == 1 && lat.bond_array[i][(j+lat_size-1)%lat_size].visited != 2) {
						 	NODE new_node = {i, (j+lat_size-1)%lat_size};
						 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
							stack.push(new_node);
						}
						if (b.right == 1 && lat.bond_array[i][(j+1)%lat_size].visited != 2) {
							NODE new_node = {i,(j+1)%lat_size};
							lat.bond_array[i][(j+1)%lat_size].visited = 2;
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

void sublattice_search(struct sub_lat* sub) {
	std::stack<NODE> S;
	int i0 = sub->x;
	int i1 = sub->x1;
	int cluster_count = 1; //Start at 1 but need to do sub.clusters.at(i-1);
	int thread = omp_get_thread_num();
	printf("Thread %d: Limits %d %d\n", thread,i0,i1);
	int k,l;
	for (k = 0; k < i1-i0; k++) {
		for (l = 0; l < lat_size; l++) {
			if (sub->bonds[k][l].visited == 1) {	
				std::vector<NODE> vec;
				sub->clusters.push_back(vec);
				//printf("Thread %d: %d  %d\n", thread,k,l);
				node_sum = 0;
				NODE root = {k,l};
				sub->bonds[k][l].visited = 2;
				S.push(root);

				while (!S.empty()) {
					NODE node = S.top();
					node_sum++;
					int i = node.position[0],j = node.position[1];
					if (i == 0 || i == (i1-i0)-1) {
						if (i == (i1-i0)-1) { 
							sub->bot_edge[j] = cluster_count;
						} else {
							sub->top_edge[j] = cluster_count;
						}
					}

					sub->clusters[cluster_count-1].push_back(node);
					S.pop();
					if (i > +1 && i < (i1-i0)-1) {
						if (sflag) {
							//printf("Thread %d: %d %d\n",thread, (i-1),j);
							if (i-1 >= i0 && sub->bonds[i-1][j].visited == 1) {
								NODE new_node = {i-1,j};
								sub->bonds[i-1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread, i,(j+lat_size-1)%lat_size);
							if (sub->bonds[i][(j+lat_size-1)%lat_size].visited == 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	sub->bonds[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n",thread, i+1,j);
							if (sub->bonds[i+1][j].visited == 1) {
								NODE new_node = {i+1,j};
								sub->bonds[i+1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread,i,(j+1)%lat_size);
							if (sub->bonds[i][(j+1)%lat_size].visited == 1) {
								NODE new_node = {i,(j+1)%lat_size};
								sub->bonds[i][(j+1)%lat_size].visited= 2;
								S.push(new_node);
							}
						} else {
							//Bond Percolation
							BOND b = sub->bonds[i][j];
							if (b.up && sub->bonds[(i-1)][j].visited != 2) {
								NODE new_node = {i-1,j};
								sub->bonds[i-1][j].visited = 2;
								S.push(new_node);
							}
							if (b.left && sub->bonds[i][(j+lat_size-1)%lat_size].visited != 2) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	sub->bonds[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							if (b.right && sub->bonds[i][(j+1)%lat_size].visited != 2) {
								NODE new_node = {i,(j+1)%lat_size};
								sub->bonds[i][(j+1)%lat_size].visited = 2;
								S.push(new_node);	
							}
							if (b.down && sub->bonds[i+1][j].visited != 2) {
								NODE new_node = {i+1,j};
								sub->bonds[i+1][j].visited = 2;
								S.push(new_node);
							}
						}
					}
				}
				cluster_count++;
			}
		}
	}
}

//Adds the sub lat to lat.bond_array
void combine_lats(struct sub_lat* sub) {
	for (int i = 0; i < sub->x1-sub->x; i++) {
		memcpy(&lat.bond_array[i+sub->x], &sub->bonds[i], lat_size*sizeof(BOND));
	}
}

//Combine sections on the bottom of the sections of work
void combine(int bot_row, struct sub_lat lat1, struct sub_lat lat2) {
	for (int i = 0; i < lat_size; i++) {
		if (lat.bond_array[bot_row][i].visited == 1 && lat.bond_array[bot_row][(i+1)%lat_size].visited) {
			int idx1 = lat1.bot_edge[i]-1,idx2 = lat2.top_edge[i]-1;
			while (lat2.clusters[idx2].size() > 0) {
				lat1.clusters[idx1].push_back(lat2.clusters[idx2].back());
				lat2.clusters[idx2].pop_back();
			}
		}
	}
}

/*
void percolates() {
	int i,j,num_clusters;
	#pragma omp parallel for
	{
		for (i = 0; i < num_clusters; i++) {
			int short = clusters[]
			for (j = 0; j < )
		}
	}
}*/

//Splits the larger lattice into sub lattices avoiding race conditions
struct sub_lat* create_sub_lat(int i0, int i1) {
	struct sub_lat* sub = (sub_lat*) malloc(sizeof(sub_lat));
	sub->x = i0;
	sub->x1 = i1;
	sub->bonds = (BOND**) malloc((i1-i0)*sizeof(BOND*));
	sub->bot_edge = (short*) malloc(lat_size*sizeof(short*));
	sub->top_edge = (short*) malloc(lat_size*sizeof(short*));

	for (int m = 0; m < (i1-i0); m++) {
		sub->bonds[m] = (BOND*) malloc(lat_size*sizeof(BOND));
		memcpy(&sub->bonds[m], &lat.bond_array[i0+m], sizeof(BOND)*lat_size);
	}
	
	//FOR TEST PURPOSES
	/*
		for (int j = 0; j < lat_size; j++) {
			sub->bot_edge[j] = 0;
			sub->top_edge[j] = 0;
		}*/
	return sub;
}

void check_cluster() {
	largest_cluster = 0;
	char percolates = 0; // false
	node_sum = 0;
	const int threads = 4;
	int arr_split = lat_size/threads;
	struct sub_lat storage[threads];
	#pragma omp parallel num_threads(threads) firstprivate(node_sum)
	{
		short th = omp_get_thread_num();
		struct sub_lat* sub = create_sub_lat(arr_split*th,arr_split*(th+1)); // Creates sub lattice, then DFS it and combine with actual lattice
		sublattice_search(sub); //searches through its given sublattice
		combine_lats(sub);
		storage[th] = *sub;
	}

	for (int i = 0; i < threads; i++) {
		combine(((i+1)*arr_split)-1,storage[i],storage[i+1%threads]);
	
	if (percolates == 1) {
			printf("Percolates\n");
		}
	}
}