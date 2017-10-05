#include "wqu_unionfind.h"
#include "lattice.h"
/**
 * An implementation of the Hoshen-Kopelman algorithm for labelling clusters
 * on a grid, based on a (Weighted, Quick Union) Union-Find algorithm.
 * @author: abraram (21518928@student.uwa.edu.au)
 * @author: samheath ()
 */

//SITE* treeNodes; //connectivity information of a site within a UF tree.


int* parentId; //contains the index of the parent
int* subtreeSize;  //the size of the subtree from a node.
std::vector<int> perc_label; //labels that might be assigned to a percolating cluster.
int len; //stores the size of the lattice.
bool no_sites_occupied; //since subtree size is being used as size of cluster, this
												//bool value is to check that there is at least one occupied
												// site, to prevent it from falsely returning 1.

/**
 * Initialize a connectionless lattice of a provided size..
 */
void init_qu_union_find(int siz)
{
	len = siz*siz; //including virtual top, bottom, left, right
	parentId = (int*)malloc(len*sizeof(int));
	subtreeSize = (int*)malloc(len*sizeof(int));
	for(int i = 0; i < len; i++)
	{
		parentId[i] = i;
		subtreeSize[i] = 1;
	}
}


void print_array_parentid()
{
	printf("[ ");
	for(int i = 0; i < len; i++)
	{
		printf(" %d ", parentId[i]);
	}
	printf("] \n");
}


void print_array_subtrees()
{
	printf("[ ");
	for(int i = 0; i < len; i++)
	{
		printf(" %d ", subtreeSize[i]);
	}

	printf("] \n");
}

/**
 * Return the root node of a given node.
 */
int find(int n)
{
	while(n != parentId[n])
		n = parentId[n];
	return n;
}


/**
 * return true if the 2 sites are connected, false otherwise.
 */
bool is_connected(int n1, int n2)
{
	return(find(n1) == find(n2));
}


/**
 * Connect 2 lattice sites within the Union Find tree.
 * To keep the tree minimal we always add the smaller subtree to
 * the larger subtree.
 */
void quick_union(int n1, int n2)
{
	//printf("pre\n");
	int rootN1 = find(n1);
	int rootN2 = find(n2);
	//printf("found root1, root2 %d %d\n", rootN1, rootN2);
	//already connected
	if(rootN1 == rootN2)
		return;
	if(subtreeSize[rootN1] >= subtreeSize[rootN2]){
		parentId[rootN2] = rootN1;
		subtreeSize[rootN1] += subtreeSize[rootN2];
	}else{
		parentId[rootN1] = rootN2;
		subtreeSize[rootN2] += subtreeSize[rootN1];
	}
}

/**
 * Destroys the dynamically allocated arrays
 */
void destroy_qu_union_find()
{
	free(parentId);
	free(subtreeSize);
	len = 0;
}

//---------------End of Union Find-----------------------------------//

/**
 * Accounts for the wrap-around underflow when applying mod.
 */
int modulo(int i, int s)
{
	if(i >= 0){return i%s;}
	else {return s+i;}
}

/**
* Return a single dimension equivalent index for a 2D array indexing.
*/
int twoDto1D(int i, int j, int siz)
{
	return ((siz*i)+j);
}

/**
* Updates the values of the matrix to label each individual cluster within it.
* must be called before calling vertical_percol_find and horizontal_perc_find.
* must be called after perform_union_find
*/
void perform_hoshen_kopelman_alg(int** lattice, int latsiz)
{
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] != 0)
				lattice[i][j] = find(twoDto1D(i, j, latsiz));
			else
				lattice[i][j] = -1;
		}
	}
}



/**
* perc_label stores all the potential cluster labels which can result in percolation
* this function ensures duplicate entries are avoided.
*/
bool check_if_exists_in_perc_label(int target)
{
	for(unsigned int i = 0; i < perc_label.size(); i++)
	{
		if(perc_label[i] == target)
			return true;
	}
	return false;
}


bool find_vertical_percolation(int** l, int siz)
{
	bool row_perc_found = false;
	for(unsigned int i = 0; i < perc_label.size(); i++)
	{
		if(row_perc_found)
			break;
		row_perc_found = true;
		int label_search = perc_label[i]; //current label being examined.
		for(int j = 0; j < siz; j++)
		{
			bool temp = false;
			for(int k = 0; k < siz; k++)
			{
				temp = (temp||(l[j][k] == label_search));
			}
			row_perc_found = row_perc_found && temp;
			if(temp == false) //1 row excluded.
				break;
		}
	}
	if(!row_perc_found){printf("[X] does NOT percolate across all cols!\n");}
	else{printf("[#] percolates across all cols!\n");}
	return row_perc_found;
}



bool find_horizontal_percolation(int** l, int siz)
{
	bool col_perc_found = false;
	for(unsigned int i = 0; i < perc_label.size(); i++)
	{
		if(col_perc_found)
			break;
		col_perc_found = true;
		int label_search = perc_label[i]; //current label being examined.
		for(int j = 0; j < siz; j++)
		{
			bool temp = false;
			for(int k = 0; k < siz; k++)
			{
				temp = (temp||(l[k][j] == label_search));
			}
			col_perc_found = col_perc_found && temp;
			if(temp == false) //1 row excluded.
				break;
		}
	}
	if(!col_perc_found){printf("[X] does NOT percolate across all rows!\n");}
	else{printf("[#] percolates across all rows!\n");}
	return col_perc_found;
}


/**
* We construct a vector which contains all the cluster labels that are
* assigned to clusters which can result in a percolation.
* We ignore repeats in this vector.
*/
void populate_percolation_label_vector(int latsiz)
{
	for(int z = 0; z < len; z++)
	{
		if(subtreeSize[z] > latsiz)
		{
			int cluster_label = find(z);
			if(!check_if_exists_in_perc_label(cluster_label))
			{
				perc_label.push_back(cluster_label);
			}
		}
	}
}

/**
* Perform union find to connect the sites into a Union Find search tree.
* Returns -1 if we can establish that there are no percolations.
* otherwise return 1.
*/
int perform_union_find(int** lattice, int latsiz)
{
	no_sites_occupied = true;
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] ==1)
			{
				no_sites_occupied = false; // at least 1 site is occupied.
				//look up.
				if(lattice[modulo(i-1, latsiz)][j] == 1)
					quick_union(twoDto1D(modulo(i-1,latsiz), j, latsiz), twoDto1D(i, j, latsiz));
				//look left.
				if(lattice[i][modulo(j-1, latsiz)] == 1)
					quick_union(twoDto1D(i, modulo(j-1, latsiz), latsiz), twoDto1D(i, j, latsiz));
			}
		}
	}
	populate_percolation_label_vector(latsiz);
	if(!no_sites_occupied)
	{
		std::sort(subtreeSize, len+subtreeSize);
		printf("[#] largest cluster size = %d\n", subtreeSize[len-1]);
	} else{
		printf("[#] largest cluster size = %d\n", 0);
	}
	if(subtreeSize[len-1] < latsiz) {printf("[X] does NOT percolate!\n"); return -1;}
	return 1;
}




/**
* Perform union find to connect the sites into a Union Find search tree.
* Returns -1 if we can establish that there are no percolations.
* otherwise return 1.
*/
int perform_union_find_m_t_2(int** lattice, int latsiz)
{
	no_sites_occupied = true;
	#pragma omp parallel for schedule(static)
	for(int i = 0; i < latsiz; i++)
	{
		#pragma omp parallel for schedule(static) firstprivate(i)
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] ==1)
			{
				no_sites_occupied = false; // at least 1 site is occupied.
				//look up.
				if(lattice[modulo(i-1, latsiz)][j] == 1)
				{
						quick_union(twoDto1D(modulo(i-1,latsiz), j, latsiz), twoDto1D(i, j, latsiz));
				}
				//look left.
				if(lattice[i][modulo(j-1, latsiz)] == 1)
				{
					quick_union(twoDto1D(i, modulo(j-1, latsiz), latsiz), twoDto1D(i, j, latsiz));
				}
			}
		}
	}

	populate_percolation_label_vector(latsiz);
	if(!no_sites_occupied)
	{
		std::sort(subtreeSize, len+subtreeSize);
		printf("[#] largest cluster size = %d\n", subtreeSize[len-1]);
	} else{
		printf("[#] largest cluster size = %d\n", 0);
	}
	if(subtreeSize[len-1] < latsiz) {printf("[X] does NOT percolate!\n"); return -1;}
	return 1;
}




/**
* Perform union find to connect the sites into a Union Find search tree.
* Returns -1 if we can establish that there are no percolations.
* otherwise return 1.
*/
int perform_union_find_multi_threaded(int** lattice, int latsiz)
{
	no_sites_occupied = true;
#pragma omp parallel
{
#pragma omp single
{
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] ==1)
			{
				no_sites_occupied = false; // at least 1 site is occupied.
				//look up.
				if(lattice[modulo(i-1, latsiz)][j] == 1)
				{
					#pragma omp task
					{
						quick_union(twoDto1D(modulo(i-1,latsiz), j, latsiz), twoDto1D(i, j, latsiz));
					}
				}
				//look left.
				if(lattice[i][modulo(j-1, latsiz)] == 1)
				{
					#pragma omp task
					{
					quick_union(twoDto1D(i, modulo(j-1, latsiz), latsiz), twoDto1D(i, j, latsiz));
					}
				}
			}
		}
	}
	#pragma omp taskwait
}
}

	populate_percolation_label_vector(latsiz);
	if(!no_sites_occupied)
	{
		std::sort(subtreeSize, len+subtreeSize);
		printf("[#] largest cluster size = %d\n", subtreeSize[len-1]);
	} else{
		printf("[#] largest cluster size = %d\n", 0);
	}
	if(subtreeSize[len-1] < latsiz) {printf("[X] does NOT percolate!\n"); return -1;}
	return 1;
}



/**
* Perform union find to connect the sites into a Union Find search tree.
* Returns -1 if we can establish that there are no percolations.
* otherwise return 1.
* Use for bond percolations.
*/
int perform_union_find_bond(BOND** bonds, int** lattice, int latsiz)
{
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] ==1)
			{
				//look up.
				if(bonds[i][j].up == 1)
					quick_union(twoDto1D(modulo(i-1,latsiz), j, latsiz), twoDto1D(i, j, latsiz));
				//look left.
				if(bonds[i][j].left == 1)
					quick_union(twoDto1D(i, modulo(j-1, latsiz), latsiz), twoDto1D(i, j, latsiz));
			}
		}
	}
	populate_percolation_label_vector(latsiz);
	std::sort(subtreeSize, len+subtreeSize);
	printf("[#] largest cluster size = %d\n", subtreeSize[len-1]);
	if(subtreeSize[len-1] < latsiz) {printf("[X] does NOT percolate!\n"); return -1;}
	return 1;
}
