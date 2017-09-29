#include <cstdio>
#include <stack>
#include "lattice.h"
#include "algorithms.h"

int siz;

/**
 * Return an int indicating what type of percolation.
 */
int find_percolation(int h[siz], int v[], int s)
{
	int horsiz = 0, vertsiz = 0;
	for(int i = 0; i < s; i++)
	{
		horsiz+=h[i];
		vertsiz+=v[i];
	}

	if(horsiz == s && vertsiz == s)
		return 2;
	if(horsiz == s)
		return 0;
	if(vertsiz == s)
		return 1;
	return -1;
}

/**
 * Performs the Depth first search over the provided graph of nodes siz^2
 * nodestack is a stack that contains the first node the stack is init. with. 
 * Returns an array of ints of size 2,
 * where the first int is -1 if non-percolating, 0 if hor. perc, 1 if vert. perc
 * and 2 if both horizontal and vertically percolating.
 * The second integer contains the number of sites that are in the cluster returned. 
 */
int* find_cluster(int graph[siz][siz], std::stack<NODE> nodestack)
{
	int horiz[siz] = {0};
	int vert[siz] = {0};
	int nodes = 0; //number of nodes traversed.
	while(!nodestack.empty())
	{
		NODE rootnode = nodestack.pop();
		int r = rootnode.row, int c = rootnode.col;
		graph[r][c] = 2;
		nodes++;
		horiz[r] = 1; vert[c] = 1;
		if(graph[(r+1)%siz][c] == 1){
			NODE downnode = {(r+1)%siz, c};
			nodestack.push(downnode);
		}
		if(graph[(r-1)%siz][c] == 1){
			NODE upnode = {(r-1)%siz, c};
			nodestack.push(upnode);
		}
		if(graph[r][(c-1)%siz] == 1){
			NODE leftnode = {r, (c-1)%siz};
			nodestack.push(leftnode);
		}
		if(graph[r][(c+1)%siz] == 1){
			NODE rightnode = {r, (c+1)%siz};
			nodestack.push(rightnode);
		}
	}
	int perc = find_percolation(int[] horiz, int[] vert, int siz);	
	int outarr[] = {perc, nodes};
	return outarr;
}


/**
 * A function that calls Depth First Search sequentially in a for loop
 * over the provided lattice to find a cluster. 
 * Must initialize array size by calling init_alg_array_size before calling
 * this function.
 */
void perform_depth_first_search(int lattice[siz][siz])
{
	int culsterstat[2];
	bool percolation = false;
	std::stack<NODE> s;
	std::vector<int> clusterlen; //stores the lens of the clusters.
        int perclen = 0, perctype = -1;
	for(int r = 0; r < len; r++)
	{
		//use goto instead??
		if(percolation){break;}
		for(int c = 0; c < len; c++)
		{
			if(lattice[r][c] == 1)
			{
				NODE root = {r,c};
				s.push(root);
				clusterstat = find_cluster(lattice, siz, s);
				//percolation found.
				if(clusterstat[0] != -1){
					percolation = true;
					perclen = clusterstat[1];
					perctype = clustestat[0];
					break;
				}else
					clusterlen.push_back(clusterstat[1]);
			}
		}
	}
	
	if(!percolation){
		 std::sort (clusterlen.begin(), clusterlen.end()); 
		 perclen = clusterlen.back();
	}
	std::sort (myvector.begin(), myvector.end(), myobject); 

	printf("percolates: %d\nmax cluster size:%d\n", perctype, perclen);
}
 
/**
 * Initializes global variable siz, which stores the lattice dimensions.
 * Must be called before perform_depth_first_search(). 
 */
void init_alg_array_size(int n)
{
	siz = n;
}

