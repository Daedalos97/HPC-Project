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
int len; //stores the size of the lattice.

/**
 * Initialize a connectionless lattice of a provided size..
 */
void init_qu_union_find(int siz)
{
	len = siz*siz; //including virtual top, bottom, left, right
	parentId = (int*)malloc(len*sizeof(int));
	subtreeSize = (int*)malloc(len*sizeof(int));

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


/**
 * Accounts for the wrap-around underflow when applying mod.
 */
int modulo(int i, int s)
{
	if(i >= 0){return i%s;}
	else {return s+i;}
}


int twoDto1D(int i, int j, int siz)
{
	return ((siz*i)+j);
}


void perform_hoshen_kopelman_alg(int** lattice, int latsiz)
{
	for(int i = 0; i < latsiz; i++)
	{
		for(int j = 0; j < latsiz; j++)
		{
			if(lattice[i][j] != 0)
				lattice[i][j] = find(twoDto1D(i, j, latsiz));
		}
	}
}

/**
* optype specifies what type of percolation is being searched..
* 1 for spanning all cols.
* 2 for spanning all rows.
*/
void perform_union_find(int** lattice, int latsiz)
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
	std::sort(subtreeSize, len+subtreeSize);
	if(subtreeSize[len-1] < latsiz) {printf("* does not percolate!");}
	printf("* largest cluster size = %d\n", subtreeSize[len-1]);
}


// bool check_if_percolates_col()
// {
// 	return is_connected(top, bottom);
// }
//
// bool check_if_percolates_row()
// {
// 	return is_connected(left, right);
// }
