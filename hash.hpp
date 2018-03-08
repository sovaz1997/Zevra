#ifndef HASH_HPP
#define HASH_HPP

#include <cstdint>
#include <stack>
#include "bitmove.hpp"

class BitMove;

class Hash {
public:
	uint8_t depth;
	int32_t score;
	uint8_t flag;
	uint16_t age;
	uint64_t key;

	uint8_t fromY, fromX, toY, toX;
	bool replaced;
	uint8_t replacedFigure;

	Hash();
	~Hash();
	void clean();
	void setMove(BitMove& mv);
	bool move_equal(BitMove& mv);
	BitMove getMove();
};
#endif



/*
Новый хеш:
	Ход - 12 бит;
	key = 64 бит;
	score = 21 бит;
	depth = 8 бит;
	age = 13 бит;
	flag = 2 бит.

	Итого: 120 бит (15 байт)
*/