#include "movearray.hpp"

MoveArray::MoveArray() : count(0), num_attacks(0) {
	moveArray.resize(300);
}

MoveArray::~MoveArray() {}

void MoveArray::clear() {
	count = 0;
	num_attacks = 0;
}

void MoveArray::addMove(BitMove mv) {
	if(count < 300 && count >= 0) {
		moveArray[count] = mv;
		++count;
	}
}
