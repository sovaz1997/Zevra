#include "movearray.hpp"

MoveArray::MoveArray() : count(0) {
	moveArray = new Move[100];
}

MoveArray::~MoveArray() {
	delete[] moveArray;
}

void MoveArray::clear() {
	count = 0;
}

void MoveArray::addMove(Move mv) {
	moveArray[count] = mv;
	++count;
}
