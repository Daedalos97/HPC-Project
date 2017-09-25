#include <stdbool.h>

typedef struct node {
	struct node* parent;
	int position[2];
} NODE;

int getSize();

bool isEmpty();

NODE* peek();

NODE* pop();

void push(NODE n);