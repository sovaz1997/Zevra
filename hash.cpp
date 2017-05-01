#include "hash.hpp"

Hash::Hash() : flag(EMPTY), prev(nullptr) {}

bool Hash::back() {
	if(prev) {
		if(!prev->empty()) {
			move = prev->top();
			prev->pop();

			return true;
		}
	}

	return false;
}

void Hash::recordPrev() {
	if(flag == EXACT) {
		if(!prev) {
			prev = new std::stack<BitMove>();
		}

		prev->push(move);
	}
}

void Hash::clean() {
	if(prev) {
		while(!prev->empty()) {
			prev->pop();
		}

		delete prev;
		prev = nullptr;
	}

	flag = EMPTY;
}
