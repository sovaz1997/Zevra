#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include "constants.hpp"
#include "category.hpp"

class BitMove {
private:
	Category category;
public:
	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	bool isAttack, replaced, passant, fromHash;

	BitMove();
	BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt);

	bool operator>(BitMove& mv);
	bool operator<(BitMove& mv);
	std::string getMoveString();
	double getAttackPrice();
	bool equal(BitMove& mv);
	void setReplaced(uint8_t figure);
	void setCategory(int cat, double val);
	bool quality();
};

#endif
