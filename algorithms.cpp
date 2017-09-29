#include "lattice.h"
#include "algorithms.h"


/**
 * Accounts for the wrap-around underflow when applying mod.
 */
int modulo(int i, int s)
{
	if(i >= 0)
		return i%s;
	else
		return s+i;
}

/**
 * Return an int indicating what type of percolation.
 */
int find_percolation(int* h, int* v, int s)
{
	int horsize = 0, vertsize = 0;
	for(int i = 0; i < s; i++)
	{
		horsize+=h[i];
		vertsize+=v[i];
	}

	if(horsize == s && vertsize == s)
		return 2;
	if(horsize == s)
		return 0;
	if(vertsize == s)
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
int* find_cluster(int** graph, int siz, std::stack<NODE> nodestack)
{
	int* horiz = (int*)malloc(siz*sizeof(int));
	int* vert = (int*)malloc(siz*sizeof(int));
	int nodes = 1; //number of nodes traversed. Could be huge for 2048 sized lattice.
	//std::cout << "node" << std::endl;
	while(!nodestack.empty())
	{
		//std::cout << "test\n";
		NODE rootnode = nodestack.top();
		nodestack.pop();
		int r = rootnode.row, c = rootnode.col;
		//std::cout <<"##popped "<< r << " " << c << std::endl;
		graph[r][c] = 2;
		//std::cout << "           " << 1 << std::endl;
		//std::cout << "       nodes:" << nodes << std::endl;
		horiz[r] = 1; vert[c] = 1;

		//std::cout << "-----------DOWN" << (r+1)%siz << " " << c << std::endl;
		//std::cout << "           "<< horiz[r] << " " << vert[c] << std::endl;
		if(graph[(r+1)%siz][c] == 1){
			//std::cout << "**" << (r+1)%siz << " " << c << std::endl;
			NODE downnode = {(r+1)%siz, c};
			graph[(r+1)%siz][c] = 2;
			//std::cout << "**pushed [down neighbour]" << (r+1)%siz << " " << c << std::endl;
			nodestack.push(downnode);
			nodes+=1;
		}
		//std::cout << "-------------UP" << modulo(r-1, siz)<< " " << c << std::endl;
		if(graph[modulo((r-1),siz)][c] == 1){
			//std::cout << "**" << modulo(r-1, siz) << " " << c << std::endl;
			NODE upnode = {modulo(r-1, siz), c};
			graph[modulo((r-1),siz)][c] = 2;
			//std::cout << "**pushed [up neighbour]" << modulo(r-1, siz)<< " " << c << std::endl;
			nodestack.push(upnode);
			nodes+=1;
		}
		//std::cout << "-------------LEFT" << r << " " <<modulo(c-1,siz) << " " <<std::endl;
		if(graph[r][modulo((c-1),siz)] == 1){
			NODE leftnode = {r, modulo(c-1, siz)};
			graph[r][modulo((c-1),siz)] = 2;
			//std::cout << "**pushed [left neighbour]" << r << " " <<modulo(c-1,siz) << " " <<std::endl;
			nodestack.push(leftnode);
			nodes+=1;
		}
		//std::cout<< "---------------RIGHT" << r << " " << (c+1)%siz << " " << std::endl;
		if(graph[r][(c+1)%siz] == 1){
			NODE rightnode = {r, (c+1)%siz};
			graph[r][(c+1)%siz] = 2;
			//std::cout<< "**pushed [right neighbour]" << r << " " << (c+1)%siz << " " << std::endl;
			nodestack.push(rightnode);
			nodes+=1;
		}
	}
	//std::cout << "fin" << std::endl;
	int perc = find_percolation(horiz, vert, siz);
	int* outarr = (int*)malloc(2*sizeof(int));
	outarr[0] = perc; outarr[1] = nodes;
	free(horiz); free(vert);
	return outarr;
}


/**
 * A function that calls Depth First Search sequentially in a for loop
 * over the provided lattice to find a cluster.
 * Must initialize array size by calling init_alg_array_size before calling
 * this function.
 */
void perform_depth_first_search(int** lattice, int siz)
{
	int* clusterstat;
	bool percolation = false;
	std::stack<NODE> s;
	std::vector<int> clusterlen; //stores the lens of the clusters.
        int perclen = 0, perctype = -1;
	for(int r = 0; r < siz; r++)
	{
		//use goto instead??
		if(percolation){break;}
		for(int c = 0; c < siz; c++)
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
					perctype = clusterstat[0];
					break;
				}else
					clusterlen.push_back(clusterstat[1]);
			}
		}
	}

	if(!percolation){
		 std::sort (clusterlen.begin(), clusterlen.end()); //sort the vector containing cluster sizes.
		 perclen = clusterlen.back(); //last element contains the largest cluster size.
	}
	free(clusterstat);
	printf("percolates: %d\nmax cluster size:%d\n", perctype, perclen);
}
