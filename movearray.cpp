#include "movearray.hpp"

MoveArray::MoveArray() : count(0), num_attacks(0) {
	moveArray.resize(288);
}

MoveArray::~MoveArray() {}

void MoveArray::clear() {
	count = 0;
	num_attacks = 0;
}

void MoveArray::addMove(BitMove mv) {
	if(count < 288 && count >= 0) {
		moveArray[count] = mv;
		++count;
	}
}

void MoveArray::setHistoryCompare(bool val) {
	for(unsigned int i = 0; i < count; ++i) {
		moveArray[i].historyCompare = val;
	}
}