#include "movearray.hpp"

MoveArray::MoveArray() : count(0), num_attacks(0) {
	moveArray.resize(60);
}

MoveArray::~MoveArray() {}

void MoveArray::clear() {
	count = 0;
	num_attacks = 0;
	moveArray.resize(60);
}

void MoveArray::addMove(BitMove mv) {
	if(count < 60) {
		moveArray[count] = mv;
	} else {
		moveArray.push_back(mv);
	}
	++count;
}

void MoveArray::setHistoryCompare(bool val) {
	for(unsigned int i = 0; i < count; ++i) {
		moveArray[i].historyCompare = val;
	}
}