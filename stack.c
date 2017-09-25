#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

NODE* stack;
int size_stack = 0;

int getSize() { return size_stack; }

bool isEmpty() { return !(size_stack > 0); }

NODE* peek() {
	if (!isEmpty()) return &stack[size_stack-1];
	else return NULL;
}

//Remember to release memory or save elsewhere after you pop.
NODE* pop() {
	if (!isEmpty()) return &stack[--size_stack];
	else return NULL;
}

void push(NODE n) {
	/*if (size_stack == 0) {
		stack = malloc(sizeof(NODE*));
		memcpy(&stack[0], &n, sizeof(NODE));
	} else {*/
		NODE* new_stack = (NODE *) realloc(stack,sizeof(NODE)*(++size_stack));
		stack = new_stack;
		stack[size_stack-1] = n;
	//}
}