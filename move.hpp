#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>
#include <string>
#include "constants.hpp"
#include "board.hpp"

class Board;

class Move {
private:
	void init(uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx, uint8_t mt);
	bool bit_quality(int bit);
public:
	Move();
	Move(uint8_t fy, uint8_t fx, uint8_t ty, uint8_t tx);
	Move(std::string mv);
	void go(Board* b);
	bool operator>(Move& mv);
	bool operator<(Move& mv);
	bool fromHash = false;

	uint8_t figureTransform;
	uint8_t passant_x, passant_y;
	uint8_t moveType;
	double simple_eval;
	void print();
	double getAttackPrice(Board & b);

	uint8_t fromY, fromX, toY, toX;

	std::string getMoveString();

	bool equal(Move & mv);

	bool quality();
};

#endif
