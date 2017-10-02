#include "search.h"
#include <vector>

std::stack<NODE> stack;

bool percolates = false;
bool end = false;
int largest_cluster = 0;

void search_lattice() {
	int* pos = (int*) malloc(2*sizeof(int));
	pos[0] = 0, pos[1] = 0;
	while (!end) {
		if (!bflag) {
			pos = find_cluster(pos);
			NODE n = {pos[0],pos[1]};
			if (check_cluster(n)) {
				percolates = true;
				break;
			}
		} else {
			pos = find_cluster_bonds(pos);
			NODE n = {pos[0],pos[1]};
			if (check_cluster_bonds(n)) {
				percolates = true;
				break;
			}
		}
	}
	if (percolates) {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	} else {
		printf("Largest Cluster is %d nodes\n", largest_cluster);
	}
	end = false;
}

/**
 *	Looks for the starting point of a cluster
 */
int* find_cluster(int* last_pos) {
	for (; last_pos[0] < lat.len; last_pos[0]++) {
		for (; last_pos[1] < lat.len; last_pos[1]++) {
			if (lat.lattice_array[last_pos[0]][last_pos[1]] == 1) {
				lat.lattice_array[last_pos[0]][last_pos[1]] = 2;
				return last_pos;
			}
		}
		last_pos[1] = 0; //stops the column from staying on the last column or past the len of lattice
	}
	end = true;
	last_pos[0] = lat.len; last_pos[1] = lat.len;
	return last_pos;
}

int* find_cluster_bonds(int* last_pos) {
	for (; last_pos[0] < lat.len; last_pos[0]++) {
		for (; last_pos[1] < lat.len; last_pos[1]++) {
			if (lat.bond_array[last_pos[0]][last_pos[1]].left == 1) {
				lat.bond_array[last_pos[0]][last_pos[1]].left = 2;
				return last_pos;
			}
			if (lat.bond_array[last_pos[0]][last_pos[1]].up == 1) {
				lat.bond_array[last_pos[0]][last_pos[1]].up = 2;
				return last_pos;
			}
			if (lat.bond_array[last_pos[0]][last_pos[1]].right == 1) {
				lat.bond_array[last_pos[0]][last_pos[1]].right = 2;
				return last_pos;
			}
			if (lat.bond_array[last_pos[0]][last_pos[1]].down == 1) {
				lat.bond_array[last_pos[0]][last_pos[1]].down = 2;
				return last_pos;
			}
		}
		last_pos[1] = 0; //stops the column from staying on the last column or past the len of lattice
	}
	end = true;
	last_pos[0] = lat.len; last_pos[1] = lat.len;
	return last_pos;
}

bool check_cluster(NODE n) {
	if (n.position[0] == lat.len || n.position[1] == lat.len) return false;
	int node_sum = 0;
	int horiz_sum = 0;
	int verti_sum = 0;
	std::vector<int> horiz (lat.len);
	std::vector<int> verti (lat.len);
	stack.push(n);
	while (!stack.empty()) {
		NODE n = stack.top();
		node_sum++;
		int i = n.position[0],j = n.position[1];
		stack.pop();
		if (!horiz.at(j)) {
			horiz.at(j) = 1;
			horiz_sum++;
		}
		if (!verti.at(i)) {
			verti.at(i) = 1;
			verti_sum++;
		}
		if (lat.lattice_array[(i+1)%lat_size][j] == 1) {
			NODE new_node = {(i+1)%lat_size,j};
			lat.lattice_array[(i+1)%lat_size][j] = 2;
			stack.push(new_node);
		}
		if (lat.lattice_array[i][(j+1)%lat_size] == 1) {
			NODE new_node = {i,(j+1)%lat_size};
			lat.lattice_array[i][(j+1)%lat_size] = 2;
			stack.push(new_node);	
		}
		if (lat.lattice_array[(i+lat_size-1)%lat_size][j] == 1) {
			NODE new_node = {(i+lat_size-1)%lat_size,j};
			lat.lattice_array[(i+lat_size-1)%lat_size][j] = 2;
			stack.push(new_node);
		}
		if (lat.lattice_array[i][(j+lat_size-1)%lat_size] == 1) {
		 	NODE new_node = {i, (j+lat_size-1)%lat_size};
		 	lat.lattice_array[i][(j+lat_size-1)%lat_size] = 2;
			stack.push(new_node);
		}
	}
	if (node_sum > largest_cluster) largest_cluster = node_sum;
	if (horiz_sum == lat.len && verti_sum == lat.len) {
		printf("Percolates Horizontally & Vertically!\n");
		return true;
	} else if (horiz_sum == lat.len) {
		printf("Percolates Horizontally!\n");
		return true;
	} else if (verti_sum == lat.len) {
		printf("\nPercolates Vertically!\n");
		return true;
	} else {
		//printf("%d\t%d\n",horiz_sum,verti_sum);
		return false;
	}
}

bool check_cluster_bonds(NODE n) {
	if (n.position[0] == lat.len || n.position[1] == lat.len) return false;
	int node_sum = 1;
	int horiz_sum = 0;
	int verti_sum = 0;
	std::vector<int> horiz (lat.len);
	std::vector<int> verti (lat.len);
	stack.push(n);
	while (!stack.empty()) {
		NODE n = stack.top();
		node_sum++;
		int i = n.position[0],j = n.position[1];
		lat.lattice_array[i][j] = 2;
		stack.pop();
		if (!horiz.at(j)) {
			horiz.at(j) = 1;
			horiz_sum++;
		}
		if (!verti.at(i)) {
			verti.at(i) = 1;
			verti_sum++;
		}
		if ((lat.bond_array[i][j].down == 1) && (lat.lattice_array[(i+1)%lat_size][j] != 2)) {
			NODE new_node = {(i+1)%lat_size,j};
			lat.lattice_array[(i+1)%lat_size][j] = 2;
			stack.push(new_node);
		}
		if ((lat.bond_array[i][j].left == 1) && (lat.lattice_array[i][(j+1)%lat_size] != 2)) {
			NODE new_node = {i,(j+1)%lat_size};
			lat.lattice_array[i][(j+1)%lat_size] = 2;
			stack.push(new_node);	
		}
		if ((lat.bond_array[i][j].up == 1) && (lat.lattice_array[(i+lat_size-1)%lat_size][j] != 2)) {
			NODE new_node = {(i+lat_size-1)%lat_size,j};
			lat.lattice_array[(i+lat_size-1)%lat_size][j] = 2;
			stack.push(new_node);
		}
		if ((lat.bond_array[i][j].right == 1) && (lat.lattice_array[i][(j+lat_size-1)%lat_size] != 2)) {
		 	NODE new_node = {i, (j+lat_size-1)%lat_size};
		 	lat.lattice_array[i][(j+lat_size-1)%lat_size] = 2;
			stack.push(new_node);
		}
	}
	if (node_sum > largest_cluster) largest_cluster = node_sum;
	if (horiz_sum == lat.len && verti_sum == lat.len) {
		printf("Percolates Horizontally & Vertically!\n");
		return true;
	} else if (horiz_sum == lat.len) {
		printf("Percolates Horizontally!\n");
		return true;
	} else if (verti_sum == lat.len) {
		printf("\nPercolates Vertically!\n");
		return true;
	} else {
		//printf("%d\t%d\n",horiz_sum,verti_sum);
		return false;
	}
}