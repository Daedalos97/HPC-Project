#include <stdbool.h>

typedef struct node {
	struct node* parent;
	int position[2];
} NODE;

extern NODE *stack;
extern int size_stack;

bool isEmpty();

NODE* peek();

NODE* pop();

void push(NODE n);