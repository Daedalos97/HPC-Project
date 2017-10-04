#include "wqu_unionfind.h"
#include "hoshen_kopelman.h"

/**
 * Accounts for the wrap-around underflow when applying mod.
 */
int modulo(int i, int s)
{
	if(i >= 0){return i%s;}
	else {return s+i;}
}


void init_hoshen_kopelman(int siz)
{
	init_qu_union_find(siz);
}


/**
 * Initializes the Union Find search tree which our Hoshen-Kopelman alg. uses.
 * Union Find algorithm to find clusters.
 * https://en.wikipedia.org/wiki/Hoshen-Kopelman_algorithm
 * args: c the character lattice with sites 0 or 1. siz, the size of the lattice.
 */
void use_h_k_algorithm_site(unsigned char** l, int siz)
{
	unsigned char largest_label = 1;
	for(int r = 0; r < siz; r++)
	{
		for(int c = 0; c < siz; c++)
		{
			if(l[r][c] == 1)
			{
			       unsigned char up_neighbor = l[modulo((r-1), siz)][c];
			       unsigned char left_neighbor = l[r][modulo((c-1), siz)];
						 //unsigned char self = l[r][c];
						 int self2d = r*(siz) + c;
						 int up2d = (modulo(r-1,siz)*(siz)) +c;
						 int l2d = r*siz + (modulo(c-1, siz));
						 printf("self %d up %d left %d\n", self2d, up2d, l2d);
			       if(up_neighbor == 0 && left_neighbor == 0)
			       {
				       largest_label++;
				       l[r][c] = largest_label;
			       }
			       else if(up_neighbor != 0 && left_neighbor == 0)
			       {
					//quick_union(self2d, up2d);
					l[r][c] =  find(up2d);
			       }
			       else if(up_neighbor == 0 && left_neighbor != 0)
			       {
				       //quick_union(self2d, l2d);
				       l[r][c] =  find(l2d);
			       }
			       else
			       {
				       //quick_union(self2d, l2d);
				       quick_union(l2d, up2d);
				       l[r][c] = find(l2d);
			       }
			}
		}
	}
	print_arrays();
	print_array_size();
}

/**
 * Initializes the Union Find search tree based on the bond probability.
 *
 */
//void use_h_k_algorithm_bond(int siz, double prob)
//{
//
//}
