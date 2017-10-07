#include "search.h"

#include <stack>

int largest_cluster = 0;
int node_sum;
bool percolate = false;
const int threads = 2;

void search_lattice() {
	check_cluster();
	printf("[#] Largest Cluster: %d\n", largest_cluster);
	if (percolate) {
		printf("[+] Percolates.\n");
	} else {
		printf("[X] Does not percolate.\n");
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
	for (i = 0; i < lat_size; i++) {
		if (sflag) {
			if (lat.bond_array[lat1->x1-1][i].visited == 2 && lat.bond_array[(lat2->x)%lat_size][i].visited == 2) {
				int idx1 = lat1->bot_edge[i]-1, idx2 = lat2->top_edge[i]-1;
				if (idx2>=0 && idx1 >= 0) {
					lat1->clusters[idx1].insert(std::end(lat1->clusters[idx1]), std::begin(lat2->clusters[idx2]), std::end(lat2->clusters[idx2]));
					lat2->clusters[idx2].resize(0);
				}
				
			}
		} else {
			//Checks a bond exists between these points
			if (lat.bond_array[lat1->x-1][i].down == 1 && lat.bond_array[lat1->x1-1][i].visited == 2 && lat.bond_array[(lat2->x)%lat_size][i].visited == 2) {
				int idx1 = lat1->bot_edge[i]-1,idx2 = lat2->top_edge[i]-1;
				if (idx2 >= 0 && idx1 >= 0) {
					lat2->clusters[idx2].insert(std::end(lat2->clusters[idx2]), std::begin(lat1->clusters[idx1]), std::end(lat1->clusters[idx1]));
					lat1->clusters[idx1].resize(0);
				}
			}
		}
	}
}

void percolates(bool perc,std::vector<struct sub_lat> lats, int num_lat) {
	int i,j,k;
	//#pragma omp parallel for num_threads(4) shared(largest_cluster)
		for (i = 0; i < num_lat; i++) {
			for (j = 0; j < (int)lats[i].clusters.size(); j++) {
				//printf("%d\n",(int)sub.clusters[j].size());
				if ((int)lats[i].clusters[j].size() > largest_cluster) {
					#pragma omp critical
					{
						largest_cluster = lats[i].clusters[j].size();
					}
				}
				if ((int)lats[i].clusters[j].size() > lat_size) {
					char* horiz = (char*) malloc(lat_size*sizeof(char));
					char* verti = (char*) malloc(lat_size*sizeof(char));
					int hori_sum = 0;
					int vert_sum = 0;
						for (k = 0; k < (int)lats[i].clusters[j].size(); k++) {
							NODE n = lats[i].clusters.at(j).at(k);
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
	sub->bot_edge = (short*) malloc(lat_size*sizeof(short*));
	sub->top_edge = (short*) malloc(lat_size*sizeof(short*));

	for (int j = 0; j < lat_size; j++) {
		sub->bot_edge[j] = 0;
		sub->top_edge[j] = 0;
	}
	return sub;
}

void check_cluster() {
	int arr_split = lat_size/threads;
	std::vector<struct sub_lat> storage(threads);
	#pragma omp parallel num_threads(threads)
	{
		short th = omp_get_thread_num();
		struct sub_lat* sub = create_sub_lat(arr_split*th,arr_split*(th+1)); // Creates sub lattice, then DFS it and combine with actual lattice
		sublattice_search(sub); //searches through its given sublattice
		storage.at(th) = *sub;
	}

	for (int i = 0; i < threads-1; i++) {
		merge_clusters(&storage[i],&storage[(i+1)%threads]);
	}
	//Merge last with first
	merge_clusters(&storage[(threads-1)], &storage[0]);

/*
	printf("\nCluster Output\n");
	for (int k = 0; k < threads; k++) {
		printf("Grid: %d\n", k);
		for (int i = 0; i < (int)storage[k].clusters.size(); i++){
			printf("Cluster: %d - size: %d\n", i, (int)storage[k].clusters[i].size());
			//for (int j = 0; j < (int)storage[k].clusters[i].size(); j++) {
				NODE n = storage[k].clusters[i].at(j);
				printf("%d  %d\n",n.position[0],n.position[1]);
			//}
		}
	}*/

	percolates(percolate, storage, threads);
}