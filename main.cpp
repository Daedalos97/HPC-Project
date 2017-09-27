#include <cstdio>
#include <cstdlib>

const int LATSIZ = 8;

//compile project:
//g++ -std=c++11 -Wall -Werror -pedantic -c  lattice.cpp
//
//alternatively you can use the provided Makefile.

int main(int argc, char** argv)
{
	//double percolation_probablity = atof(argc[1]);
	//seed_sites();
	double r = ((double) rand() / (RAND_MAX)) + 1;
	printf("%f\n",r);
	printf("%d \u2588#\u2588 \u2588 \u2588 \n",219);
}
