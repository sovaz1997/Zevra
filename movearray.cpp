#include "movearray.hpp"

MoveArray::MoveArray() : count(0) {
	moveArray = new MoveArray[100];
}

MoveArray::~MoveArray() {
	delete[] moveArray;
}

void MoveArray::clear() {
	count = 0;
}

void MoveArray::addMove(BitMove mv) {
	moveArray[count] = mv;
	++count;
}
