#include "wqu_unionfind.h"
/**
 * An implementation of the Hoshen-Kopelman algorithm for labelling clusters
 * on a grid, based on a (Weighted, Quick Union) Union-Find algorithm.
 * @author: abraram (21518928@student.uwa.edu.au)
 * @author: samheath ()
 */

//SITE* treeNodes; //connectivity information of a site within a UF tree.


int* parentId; //contains the index of the parent
int* subtreeSize;  //the size of the subtree from a node.
// bool row_perc_found;
// bool col_perc_found;
std::vector<int> perc_label; //labels that might be assigned to a percolating cluster.
int len; //stores the size of the lattice.
//std::map<int, int*> horiz_percol_map; // keeps an array for storing all the horiz. index.
//std::map<int, int*> vert_percol_map; // keeps an array for storing all the vertical index.


/**
 * Initialize a connectionless lattice of a provided size..
 */
void init_qu_union_find(int siz)
{
	len = siz*siz; //including virtual top, bottom, left, right
	parentId = (int*)malloc(len*sizeof(int));
	subtreeSize = (int*)malloc(len*sizeof(int));
	//row_perc_found = false;
	//col_perc_found = false;
	//treeNodes = (SITE*)malloc(siz*siz*sizeof(SITE));
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
*
*/
// int vertical_percol_find()
// {
//
// }

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
	return col_perc_found;
}


/**
* Perform union find to connect the sites into a Union Find search tree.
* Returns -1 if we can establish that there are no percolations.
* otherwise return 1.
*/
int perform_union_find(int** lattice, int latsiz)
{
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] ==1)
			{
				// if(i == 0)
				// 	quick_union(top, twoDto1D(i, j, latsiz)); //connect to virtual top.
				// if(i == latsiz-1)
				// 	quick_union(twoDto1D(i, j, latsiz), bottom); //connect to virtual bottom.

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
	//print_array_parentid();
	//print_array_subtrees();
	// print_array_rows();
	// print_array_cols();
	for(int z = 0; z < len; z++)
	{
		if(subtreeSize[z] > latsiz)
		{
			//int* horiz_array = (int*)malloc(latsiz*sizeof(int));
			//int* vert_array = (int*)malloc(latsiz*sizeof(int));
			int cluster_label = find(z);
			//printf("%d %zu\n", cluster_label, perc_label.size());
			if(!check_if_exists_in_perc_label(cluster_label))
			{
				//printf("----pushed : %d %zu\n", find(z), perc_label.size());
				perc_label.push_back(cluster_label);
			}
		}
	}

	std::sort(subtreeSize, len+subtreeSize);
	printf("[\u2714] largest cluster size = %d\n", subtreeSize[len-1]);
	if(subtreeSize[len-1] < latsiz) {printf("[ ] does not percolate!\n"); return -1;}
	return 1;
}
