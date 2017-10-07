#include "search.h"

#include <stack>

int largest_cluster = 0;
//std::stack<NODE> S;
int node_sum;

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
	int k,l;
	for (k = i0; k < i1; k++) {
		for (l = 0; l < lat_size; l++) {
			if (lat.bond_array[k][l].visited == 1) {	
				std::vector<NODE> vec;
				sub->clusters.push_back(vec);
				NODE root = {k,l};
				lat.bond_array[k][l].visited = 2;
				S.push(root);

				while (!S.empty()) {
					NODE node = S.top();
					int i = node.position[0],j = node.position[1];
					if (i == i0 || i == i1-1) {
						if (i == i1-1) { 
							sub->bot_edge[j] = cluster_count;
						} else {
							sub->top_edge[j] = cluster_count;
						}
					}

					sub->clusters.at(cluster_count-1).push_back(node);
					S.pop();
					//THIS EXCLUDES SOME OF THE TOP ROW
					//i1-1 is allocated limit
					if ((i >= i0 && i <= i1-1)) {
						if (sflag) {
							//printf("Thread %d: %d %d\n",thread, (i-1),j);
							if (i-1 >= i0 && lat.bond_array[i-1][j].visited == 1) {
								NODE new_node = {i-1,j};
								lat.bond_array[i-1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread, i,(j+lat_size-1)%lat_size);
							if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n",thread, i+1,j);
							if (i+1 < i1-1 && lat.bond_array[i+1][j].visited == 1) {
								NODE new_node = {i+1,j};
								lat.bond_array[i+1][j].visited = 2;
								S.push(new_node);
							}
							//printf("Thread %d: %d %d\n", thread,i,(j+1)%lat_size);
							if (lat.bond_array[i][(j+1)%lat_size].visited == 1) {
								NODE new_node = {i,(j+1)%lat_size};
								lat.bond_array[i][(j+1)%lat_size].visited= 2;
								S.push(new_node);
							}
						} else {
							//Bond Percolation
							BOND b = lat.bond_array[i][j];
							if (i-1 >= i0 && b.up && lat.bond_array[(i-1)][j].visited != 2) {
								NODE new_node = {i-1,j};
								lat.bond_array[i-1][j].visited = 2;
								S.push(new_node);
							}
							if (b.left && lat.bond_array[i][(j+lat_size-1)%lat_size].visited != 2) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							if (b.right && lat.bond_array[i][(j+1)%lat_size].visited != 2) {
								NODE new_node = {i,(j+1)%lat_size};
								lat.bond_array[i][(j+1)%lat_size].visited = 2;
								S.push(new_node);	
							}
							if (i+1 < i1-1 && b.down && lat.bond_array[i+1][j].visited != 2) {
								NODE new_node = {i+1,j};
								lat.bond_array[i+1][j].visited = 2;
								S.push(new_node);
							}
						}
					}
				}
				//printf("%ld\n", sub->clusters[cluster_count-1].size());
				cluster_count++;
			}
		}
	}
}

//Adds the sub lat to lat.bond_array
void combine_lats(struct sub_lat* sub) {
	for (int i = 0; i < (sub->x1)-(sub->x); i++) {
		//memcpy(&lat.bond_array[i+sub->x], &sub->bonds[i], lat_size*sizeof(BOND));
	}
}

//Combine sections on the bottom of the sections of work
void merge_clusters(struct sub_lat* lat1, struct sub_lat* lat2) {
	int i;
	//int split = lat1.x1-lat1.x;
	//#pragma omp parallel for num_threads(4) shared(lat1, lat2) schedule(static,split)
		for (i = 0; i < lat_size; i++) {
			if (lat.bond_array[lat1->x1-1][i].visited == 2 && lat.bond_array[(lat2->x)%lat_size][i].visited == 2) {
				//printf("%d-%d  %d\n", lat1->x1-1,lat2->x, i);
				int idx1 = lat1->bot_edge[i]-1,idx2 = lat2->top_edge[i]-1;
				if (idx2>0 && idx1 > 0) {
					//printf("\n%d  %d\n", (int)lat1->clusters[idx1].size(), (int)lat2->clusters[idx2].size());
					lat1->clusters[idx1].insert(std::end(lat1->clusters[idx1]), std::begin(lat2->clusters[idx2]), std::end(lat2->clusters[idx2]));
					lat2->clusters[idx2].resize(0);
					//printf("\n%d\n", (int)lat1->clusters[idx1].size());
				}
				
			}
		}
}

void percolates(bool perc,struct sub_lat* lats, int num_lat) {
	//printf("\n");
	int i,j,k;
	//#pragma omp parallel for num_threads(8) shared(largest_cluster)
		for (i = 0; i < num_lat; i++) {
			struct sub_lat sub = lats[i];
			for (j = 0; j < (int)sub.clusters.size(); j++) {
				//printf("%d\n",(int)sub.clusters[j].size());
				if ((int)sub.clusters[j].size() > largest_cluster) {
					#pragma omp critical
					{
						largest_cluster = sub.clusters[j].size();
					}
				}
				if ((int)sub.clusters[j].size() > lat_size) {
					char* horiz = (char*) malloc(lat_size*sizeof(char));
					char* verti = (char*) malloc(lat_size*sizeof(char));
					int hori_sum = 0;
					int vert_sum = 0;
						for (k = 0; k < (int)sub.clusters[j].size(); k++) {
							NODE n = sub.clusters.at(j).at(k);
							if (!horiz[n.position[1]]) {
								horiz[n.position[1]] = 1;
								hori_sum++;
							}
							if (!verti[n.position[0]]) {
								verti[n.position[0]] = 1;
								vert_sum++;
							}
						}
					if (vert_sum == lat_size && hori_sum == lat_size && matchtype == 2) {
						perc = true;
					} else if (hori_sum == lat_size && matchtype == 1) {
						perc = true;
					} else if (vert_sum == lat_size && matchtype == 0) {
						perc = true;
					} 
				}
			}
		}
}

//Splits the larger lattice into sub lattices avoiding race conditions
struct sub_lat* create_sub_lat(int i0, int i1) {
	struct sub_lat* sub = (sub_lat*) malloc(sizeof(sub_lat));
	sub->x = i0;
	sub->x1 = i1;
	//sub->bonds = (BOND**) malloc((i1-i0)*sizeof(BOND*));
	sub->bot_edge = (short*) malloc(lat_size*sizeof(short*));
	sub->top_edge = (short*) malloc(lat_size*sizeof(short*));

	/*for (int m = 0; m < (i1-i0); m++) {
		sub->bonds[m] = (BOND*) malloc(lat_size*sizeof(BOND));
		memcpy(&sub->bonds[m], &lat.bond_array[i0+m], sizeof(BOND)*lat_size);
	}*/

	//FOR TEST PURPOSES
	
		for (int j = 0; j < lat_size; j++) {
			sub->bot_edge[j] = 0;
			sub->top_edge[j] = 0;
		}
	return sub;
}

void check_cluster() {
	bool percol = false; // false
	const int threads = 8;
	int arr_split = lat_size/threads;
	struct sub_lat* storage = (sub_lat*)malloc(threads*sizeof(struct sub_lat));
	#pragma omp parallel num_threads(threads)
	{
		short th = omp_get_thread_num();
		struct sub_lat* sub = create_sub_lat(arr_split*th,arr_split*(th+1)); // Creates sub lattice, then DFS it and combine with actual lattice
		sublattice_search(sub); //searches through its given sublattice
		storage[th] = *sub;
	}

//CHECK THIS!!!!!!
	for (int i = 0; i < threads; i++) {
		merge_clusters(&storage[i],&storage[(i+1)%threads]);
	}
	percolates(percol, storage, threads);

	if (percol) {
		printf("Percolates\n");
	}
}