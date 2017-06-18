#include "hash.hpp"

Hash::Hash() :/* prev(nullptr),*/ flag(EMPTY), age(0) {}

Hash::~Hash() {
	clean();
}
/*
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
}*/

void Hash::clean() {
	/*if(prev) {
		while(!prev->empty()) {
			prev->pop();
		}

		delete prev;
		prev = nullptr;
	}*/

	flag = EMPTY;
}

void Hash::setMove(BitMove& mv) {
	fromY = mv.fromY;
	fromX = mv.fromX;
	toY = mv.toY;
	toX = mv.toX;
	replaced = mv.replaced;
	replacedFigure = mv.replacedFigure;
}

bool Hash::move_equal(BitMove& mv) {
	//return mv.equal(move);
	return (mv.fromY == fromY && mv.fromX == fromX && mv.toY == toY && mv.toX == toX && (!mv.replaced || mv.replacedFigure == replacedFigure));
}

BitMove Hash::getMove() {
	BitMove result = BitMove(0, fromY, fromX, toY, toX);
	result.setReplaced(replacedFigure);
	return result;
}