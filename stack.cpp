#include "stack.h"

#include <cstddef>
#include <stdlib.h>

std::vector<NODE> stack (0);

int getSize() { return stack.size(); }

bool isEmpty() { return stack.size() > 0;}

NODE* peek() {
	if (!isEmpty()) {
		return &stack[getSize()-1];
	} else return NULL;
}

NODE* pop() {
	NODE *n = (NODE*) malloc(sizeof(NODE));
	n = &stack[getSize()-1];
	stack.resize(getSize()-1);
	return n;
}

void push(NODE n) {
	stack.resize (getSize()+1);
	stack[getSize()-2] = n;
}