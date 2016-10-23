#include "movearray.hpp"

MoveArray::MoveArray() : count(0), num_attacks(0) {
	//moveArray = new BitMove[100];
	moveArray.resize(109);
}

MoveArray::~MoveArray() {}

void MoveArray::clear() {
	count = 0;
	num_attacks = 0;
}

void MoveArray::addMove(BitMove mv) {
	moveArray[count] = mv;
	++count;
}
