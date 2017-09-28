#include <vector>
#include <stdbool.h>

typedef struct node {
	int position[2];
} NODE;

extern std::vector<NODE> stack;

bool isEmpty();

NODE* peek();

NODE* pop();

void push(NODE n);