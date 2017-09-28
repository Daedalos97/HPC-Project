#include "stack.h"

#include <cstddef>
#include <stdlib.h>

std::vector<NODE> stack (0);

int get_size() { return stack.size(); }

bool is_empty() { return stack.size() > 0;}

NODE* peek() {
	if (!is_empty()) {
		return &stack[get_size()-1];
	} else return NULL;
}

NODE* pop() {
	NODE *n = (NODE*) malloc(sizeof(NODE));
	n = &stack[get_size()-1];
	stack.resize(get_size()-1);
	return n;
}

void push(NODE n) {
	stack.resize (get_size()+1);
	stack[get_size()-1] = n;
}