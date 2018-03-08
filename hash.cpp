#include "hash.hpp"

Hash::Hash() : flag(EMPTY), age(0) {}

Hash::~Hash() {
	clean();
}

void Hash::clean() {
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
	return (mv.fromY == fromY && mv.fromX == fromX && mv.toY == toY && mv.toX == toX && (!mv.replaced || mv.replacedFigure == replacedFigure));
}

BitMove Hash::getMove() {
	BitMove result = BitMove(0, fromY, fromX, toY, toX);
	result.setReplaced(replacedFigure);
	return result;
}