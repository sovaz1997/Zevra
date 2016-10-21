#ifndef MOVEARRAY_HPP
#define MOVEARRAY_HPP

#include "bitmove.hpp"

class BitMove;

class MoveArray {
public:
	BitMove* moveArray;
	MoveArray();
	~MoveArray();
	int count;
	void clear();
	void addMove(BitMove mv);
};

#endif
