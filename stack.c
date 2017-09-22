#include <string.h>
#include <stdlib.h>

#include "stack.h"

NODE* stack = NULL;
int size_stack = 0;

bool isEmpty() { return !(size_stack > 0); }

NODE* peek() {
	if (!isEmpty()) return &stack[size_stack-1];
	else return NULL;
}

//Remember to release memory or save elsewhere after you pop.
NODE* pop() {
	if (!isEmpty()) {
		return &stack[--size_stack];
	} else return NULL;
}

void push(NODE n) {
	NODE* new_stack = (NODE *) realloc(stack,sizeof(NODE)*(++size_stack));
	stack = new_stack;
	stack[size_stack-1] = n;
}