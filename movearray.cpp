#include "movearray.hpp"

MoveArray::MoveArray() : count(0), num_attacks(0) {
	moveArray.resize(MAX_SIZE);
}

MoveArray::~MoveArray() {}

void MoveArray::clear() {
	count = 0;
	num_attacks = 0;
}

void MoveArray::addMove(BitMove mv) {
	if(count < MAX_SIZE) {
		moveArray[count] = mv;
		++count;
	}
}
