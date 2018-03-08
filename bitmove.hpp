#ifndef BITMOVE_HPP
#define BITMOVE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <cmath>
#include <vector>
#include "constants.hpp"

const int attackedPrice[6] = {0, PAWN_EV.mg, KNIGHT_EV.mg, BISHOP_EV.mg, ROOK_EV.mg, QUEEN_EV.mg};

class BitMove {
public:
	uint8_t attackedFigure, movedFigure, fromY, fromX, toY, toX;
	uint8_t replacedFigure;
	bool isAttack, replaced, passant, fromHash;
	double history_weight;
	bool historyCompare = false;

	BitMove();
	BitMove(uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	BitMove(uint8_t afig, uint8_t fig, uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, bool psnt);

	bool operator>(const BitMove& mv) const;
	bool operator<(const BitMove& mv) const;
	std::string getMoveString();
	int getAttackPrice() const;
	bool equal(BitMove& mv);
	void setReplaced(uint8_t figure);
	bool quality();
};

#endif
