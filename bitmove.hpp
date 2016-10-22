#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include "constants.hpp"

class BitMove {
public:
	uint8_t fromY, fromX, toY, toX;
	uint8_t movedFigure, attackedFigure;
	bool isAttack;

	BitMove();
	BitMove(uint8_t fy, uint8_t fig, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	std::string getMoveString();
	double getAttackPrice();
	bool equal(BitMove& mv);
};

#endif
