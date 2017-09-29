#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <stack>
#include <vector>
#include <algorithm>

//represeting a node in the provided 2D graph. 
typedef struct node{
	int row;
	int col;
} NODE;

extern void perform_depth_first_search(int** l, int s);

#endif
