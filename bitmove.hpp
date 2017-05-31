#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <vector>
#include "constants.hpp"
#include "category.hpp"


//std::vector<uint8_t>figureToIndex;

/*
void setBitMoveConstants() {
	figureToIndex.resize(32);
	figureToIndex[EMPTY] = 0;
	figureToIndex[KING | WHITE] = 1;
	figureToIndex[QUEEN | WHITE] = 2;
	figureToIndex[ROOK | WHITE] = 3;
	figureToIndex[BISHOP | WHITE] = 4;
	figureToIndex[KNIGHT | WHITE] = 5;
	figureToIndex[PAWN | WHITE] = 6;
	figureToIndex[KING | BLACK] = 7;
	figureToIndex[QUEEN | BLACK] = 8;
	figureToIndex[ROOK | BLACK] = 9;
	figureToIndex[BISHOP | BLACK] = 10;
	figureToIndex[KNIGHT | BLACK] = 11;
}
*/	

class BitMove {
private:
	//Category category;
public:
	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	uint32_t history_weight;
	bool isAttack, replaced, passant, fromHash;

	BitMove();
	BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt);

	bool operator>(const BitMove& mv) const;
	bool operator<(const BitMove& mv) const;
	std::string getMoveString();
	double getAttackPrice() const;
	bool equal(BitMove& mv);
	void setReplaced(uint8_t figure);
	void setCategory(int cat, double val);
	bool quality();
	bool operator<(BitMove& left);
};

/*
Новый BitMove:

4 bits * 3 - figures;
6 bit - fromY/fromX;
6 bit-toY/toX
4 bit - other flags
All: uint32_t (28 bit)
*/

#endif
