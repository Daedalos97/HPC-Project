#include "search.h"

#include <stack>

int largest_cluster = 0;
int node_sum;
bool percolate = false;
const int threads = 8;
std::vector<std::vector<NODE>> clusters;
int global_count = 0;

void dfs_traversal() {
	if (mflag) {
		check_cluster_multithreaded();
		printf("[#] Largest Cluster: %d\n", largest_cluster);
		if (percolate) {
			printf("[#] Percolates.\n");
		} else {
			printf("[X] Does not percolate.\n");
		}
	} else {
		check_cluster_linear();
		printf("[#] Largest Cluster: %d\n", largest_cluster);
		if (percolate) {
			printf("[#] Percolates.\n");
		} else {
			printf("[X] Does not percolate.\n");
		}
	}
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
				//zeros arrays
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
					percolate = true;
				} else if (horiz_sum == lat_size && matchtype == 1) {
					percolate = true;
				} else if (verti_sum == lat_size && matchtype == 0) {
					percolate = true;
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
					//i1-1 is allocated limit
					if ((i >= i0 && i < i1)) {
						if (sflag) {
							if (i-1 >= i0 && lat.bond_array[i-1][j].visited == 1) {
								NODE new_node = {i-1,j};
								lat.bond_array[i-1][j].visited = 2;
								S.push(new_node);
							}
							if (lat.bond_array[i][(j+lat_size-1)%lat_size].visited == 1) {
							 	NODE new_node = {i, (j+lat_size-1)%lat_size};
							 	lat.bond_array[i][(j+lat_size-1)%lat_size].visited = 2;
								S.push(new_node);
							}
							if (i+1 < i1 && lat.bond_array[i+1][j].visited == 1) {
								NODE new_node = {i+1,j};
								lat.bond_array[i+1][j].visited = 2;
								S.push(new_node);
							}
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
							if (i+1 < i1 && b.down && lat.bond_array[i+1][j].visited != 2) {
								NODE new_node = {i+1,j};
								lat.bond_array[i+1][j].visited = 2;
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

//Combine sections on the bottom of the sections of work
void merge_clusters(struct sub_lat* lat1, struct sub_lat* lat2) {
	int i;
	//Create a lookup array that stores for 1 cluster the corresponding cluster index in the first part of the lattice after being moved
	int *lookup = (int*) malloc(lat2->clusters.size()*sizeof(int));
	for (int i = 0; i < 2; i ++) {
		lookup[i] = -1; //False;
	}
	i = 0;
	if (!lookup[0]) printf("False\n");
	for (i = 0; i < lat_size; i++) {
		if (sflag) {
			if (lat.bond_array[lat1->x1-1][i].visited == 2 && lat.bond_array[(lat2->x)%lat_size][i].visited == 2) {
				int idx1 = lat1->bot_edge[i]-1, idx2 = lat2->top_edge[i]-1;
				if (idx2>=0 && idx1 >= 0) {
					if (lat2->clusters[idx2].size() > 0) {
						//Merge the clusters that allign accross the boundary merging into the first and growing sublattice.
						lat1->clusters[idx1].insert(std::end(lat1->clusters[idx1]), std::begin(lat2->clusters[idx2]), std::end(lat2->clusters[idx2]));
						lat2->clusters[idx2].resize(0);
						
						lookup[idx2] = idx1; //Set the lookup reference
					}
				}
				
			}
		} else {
			//Checks a bond exists between these points
			if (lat.bond_array[lat1->x1-1][i].down == 1 && lat.bond_array[lat1->x1-1][i].visited == 2 && lat.bond_array[(lat2->x)%lat_size][i].visited == 2) {
				int idx1 = lat1->bot_edge[i]-1,idx2 = lat2->top_edge[i]-1;
				if (idx2 >= 0 && idx1 >= 0 && (lookup[idx2] == -1)) {
					lat2->clusters[idx2].insert(std::end(lat2->clusters[idx2]), std::begin(lat1->clusters[idx1]), std::end(lat1->clusters[idx1]));
				}
			}
		}
	}
	/**
	 * Go along the bottom edge of the second lattice and if the cluster in there has been seen before on the top row, we update the bottom rows
	 * reference to an index in lat1 which becomes a "super" lattice. Otherwise if the cluster is on the boundary but not seen before then just
	 * add that cluster to super lattice and update the mapping index.
	 */
	for (int i = 0; i < lat_size; i++) {

		/*
		*CAREFUL OF THIS
		*/
		if (lat2->bot_edge[i] >= 0) {
			if (lookup[lat2->bot_edge[i]] == -1) { //This means that there has been no link made.
				if (lat2->clusters[lat2->bot_edge[i]].size() > 0) {
					int idx2 = lat2->bot_edge[i];
					
					lat1->clusters.push_back(lat2->clusters[idx2]);
					
					lat2->clusters[idx2].resize(0);
					lookup[lat2->bot_edge[i]] = lat1->clusters.size();
					lat2->bot_edge[i] = lat1->clusters.size(); //Update the bottom edge's reference
				}
			} else {
				lat2->bot_edge[i] = lookup[lat2->bot_edge[i]]; //Maps between the two.
			}
		}
	}
	//Merge unused clusters across
	for (int i = 0; i < (int)lat2->clusters.size(); i++) {
		if (lat2->clusters[i].size() > 0)
			lat1->clusters.push_back(lat2->clusters[i]); //take cluster and save it in the first at the end.
	}

	lat1->x1 = lat2->x1; //replace the extent of the first segment to encompass the last
	printf("New boundary: %d\n", lat1->x1);
	memcpy(lat1->bot_edge,lat2->bot_edge,lat_size*(sizeof(int)));
	lat1->bot_edge = lat2->bot_edge;
}

void percolates(bool* perc,struct sub_lat lat, int num_lat) {
	int j,k;
	for (j = 0; j < (int)lat.clusters.size(); j++) {
		if ((int)lat.clusters[j].size() > largest_cluster) {
			largest_cluster = lat.clusters[j].size();
		}
		if ((int)lat.clusters[j].size() > lat_size) {
			char* horiz = (char*) malloc(lat_size*sizeof(char));
			char* verti = (char*) malloc(lat_size*sizeof(char));
			for (int m = 0; m < lat_size; m++) {
				horiz[m] = 0;
				verti[m] = 0;
			}
			int hori_sum = 0;
			int vert_sum = 0;
			for (k = 0; k < (int)lat.clusters[j].size(); k++) {
				NODE n = lat.clusters.at(j).at(k);
				if (horiz[n.position[1]] == 0) {
					horiz[n.position[1]] = 1;
					hori_sum++;
				}
				if (verti[n.position[0]] == 0) {
					verti[n.position[0]] = 1;
					vert_sum++;
				}
			}
			if (vert_sum == lat_size && hori_sum == lat_size && matchtype == 2) {
				*perc = true;
			} else if (hori_sum == lat_size && matchtype == 1) {
				*perc = true;
			} else if (vert_sum == lat_size && matchtype == 0) {
				*perc = true;
			} 
		}
	}
}

//Splits the larger lattice into sub lattices avoiding race conditions
struct sub_lat* create_sub_lat(int i0, int i1) {
	struct sub_lat* sub = (sub_lat*) malloc(sizeof(sub_lat));
	sub->x = i0;
	sub->x1 = i1;
	sub->bot_edge = (int*) malloc(lat_size*sizeof(int*));
	sub->top_edge = (int*) malloc(lat_size*sizeof(int*));

	for (int j = 0; j < lat_size; j++) {
		sub->bot_edge[j] = 0;
		sub->top_edge[j] = 0;
	}
	return sub;
}

void check_cluster_multithreaded() {
	int arr_split = lat_size/threads;
	std::vector<struct sub_lat> storage(threads);
	#pragma omp parallel num_threads(threads)
	{
		short th = omp_get_thread_num();
		struct sub_lat* sub = create_sub_lat(arr_split*th,arr_split*(th+1)); // Creates sub lattice, then DFS it and combine with actual lattice
		sublattice_search(sub); //searches through its given sublattice
		storage.at(th) = *sub;
	}

	for (int i = 1; i < threads; i++) {
		merge_clusters(&storage[0],&storage[i]); //we slowly build up the size of 1 overtime.
	}

	percolates(&percolate, storage[0], threads);
}