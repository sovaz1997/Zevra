#include "hash.hpp"

Hash::Hash() : flag(EMPTY) {}

bool Hash::back() {
	if(!prev.empty()) {
		//move = prev.top();
		prev.pop();
		
		return true;
	}
	
	return false;
}


void Hash::recordPrev() {
	prev.push(move);
}

void Hash::clean() {
	flag = EMPTY;
	
	while(!prev.empty()) {
		prev.pop();
	}
}
